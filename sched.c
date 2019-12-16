
struct sched_entity {
    ...
    u64 exec_start;
    u64 sum_exec_runtime;
    u64 vruntime;
    u64 prev_sum_exec_runtime;
    ...
}


asmlinkage void schedule(void)
{
	//printk(KERN_INFO "schedule (%d)\n", current->pid);
	struct task_struct *prev, *next;
	mlfq_t *p_mlfq;
	runqueue_t *rq;
	list_t *queue;
	int idx;
	int yieldOrWait = -1; // 0 for yield and 1 for Wait (I/O)

	BUG_ON(!current->active_mm);
need_resched_back:
	prev = current;

	if (unlikely(in_interrupt())) {
		printk("Scheduling in interrupt\n");
		BUG();
	}

	release_kernel_lock(prev, smp_processor_id());
	rq = this_rq();
	spin_lock_irq(&rq->lock);

	switch (prev->state) {
		case TASK_INTERRUPTIBLE:
			//printk(KERN_INFO "TASK_INTERRUPTIBLE");
			if (signal_pending(prev)) {
				prev->state = TASK_RUNNING;
				break;
			}
		case TASK_UNINTERRUPTIBLE:
			//printk(KERN_INFO "TASK_UNINTERRUPTIBLE");
			yieldOrWait = 1;

		default: {
			//printk(KERN_INFO "default");
			deactivate_task(prev, rq);
			break;
		}

		case TASK_RUNNING: {
			//printk(KERN_INFO "TASK_RUNNING (counter = %d)", prev->counter);
			if (prev->counter > 0)
				yieldOrWait = 0;
		}
	}

	if (unlikely(!rq->nr_running)) {
		//printk(KERN_INFO "Turn to idle task");
		next = rq->idle;
		goto switch_tasks;
	}

	if (prev != rq->idle) {
		if (yieldOrWait == 0) {
			// If counter is not yet zero and state is TASK_RUNNING it implies the task
			// gave up to cpu before expiration of time-slice..so we schedule it in RR fashion.
			dequeue_task(prev, rq->p_mlfq);

			// Restore its timeslice
			prev->counter = PRIO_TO_TIMESLICE(current->priority);
			//printk(KERN_INFO "Yield process %d to queue %d timeslice %d", current->pid, current->priority, current->counter);

			enqueue_task(prev, rq->p_mlfq);
		} else if (yieldOrWait == 1) {

			// IF the task is waiting (Interruptable or uninterruptable)..
			// for I/O or something then upgrade its queue.
			// No need to make chages to queue as it is deactivated already.
			prev->priority = prev->priority - 1;
			if (prev->priority < 0) {
				prev->priority = 0;
			}

			//printk(KERN_INFO "Task going to wait for i/o");
		}

		// Do priority parenting stuff
		if ((prev->state == TASK_INTERRUPTIBLE) || (prev->state == TASK_UNINTERRUPTIBLE)) {
			if (prev->waiting_on != NULL) {
				if (prev->priority < prev->waiting_on->holder->priority){
					do_priority_parenting(prev, prev->waiting_on->holder);
				}
			}
		} else if ((prev->state == TASK_RUNNING) && (prev->waiting_on != NULL)) {
			undo_priority_parenting(prev->waiting_on->holder);
		}

	}

	p_mlfq = rq->p_mlfq;
	idx = sched_find_first_zero_bit(p_mlfq->bitmap);
	queue = p_mlfq->queue + idx;
	next = list_entry(queue->next, task_t, run_list);

switch_tasks:
	prev->need_resched = 0;

	if (unlikely(prev == next)) {
		//printk(KERN_INFO "Same process");
		goto same_process;
	}

	kstat.context_swtch++;
	/*
	 * there are 3 processes which are affected by a context switch:
	 *
	 * prev == .... ==> (last => next)
	 *
	 * It's the 'much more previous' 'prev' that is on next's stack,
	 * but prev is set to (the just run) 'last' process by switch_to().
	 * This might sound slightly confusing but makes tons of sense.
	 */
	rq->nr_switches++;
	rq->curr = next;
	next->processor = prev->processor;

	prepare_to_switch();
	{
		struct mm_struct *mm = next->mm;
		struct mm_struct *oldmm = prev->active_mm;
		if (!mm) {
			BUG_ON(next->active_mm);
			next->active_mm = oldmm;
			atomic_inc(&oldmm->mm_count);
			enter_lazy_tlb(oldmm, next, smp_processor_id());
		} else {
			BUG_ON(next->active_mm != mm);
			switch_mm(oldmm, mm, next, smp_processor_id());
		}

		if (!prev->mm) {
			prev->active_mm = NULL;
			mmdrop(oldmm);
		}
	}

	/*
	 * This just switches the register state and the
	 * stack.
	 */
	switch_to(prev, next, prev);
	barrier();
	rq = this_rq();

same_process:
	spin_unlock_irq(&rq->lock);
	reacquire_kernel_lock(current);

	if (current->need_resched)
		goto need_resched_back;

	//printk(KERN_INFO "~schedule\n");
	return;
}

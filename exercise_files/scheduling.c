#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/uaccess.h> 
#include <linux/slab.h> 
#include <linux/d_params.h>

/* given struct */
/*
struct d_params {
    	long deadline_1;	
	long deadline_2;
	long computation_time;	
};*/

/* set_scheduling_params initialization */

asmlinkage long sys_set_scheduling_params(long deadline_1, long deadline_2, long computation_time){
    	struct task_struct * task=current;

	/* times must be positive numbers d1<d2 */

	if(deadline_1 < 0 || deadline_2 < 0 || computation_time <= 0 || deadline_1 >= deadline_2)
		return -EINVAL;

	/* update the fiels of current task */
    
	long current_time=get_seconds();

    	task->deadline_1=deadline_1+current_time;
    	task->deadline_2=deadline_2+current_time;
    	task->computation_time=computation_time;

	/* debug message */

    	//printk("set_scheduling_params with PID:%d (d1: %ld, d2: %ld, comp_time: %ld)\n",task->pid, d1, d2, comp_time);

	printk(KERN_INFO "Name: Ioannis Chatziantoniou , AM: csd5193 , System call: sys_set_scheduling_params\n");

    	return 0; 
}

/* get_scheduling_params initialization */

asmlinkage long sys_get_scheduling_params(struct d_params *params){
    	struct task_struct * task=current;	/* current process */
    	struct d_params local;

	/* if pointer is null then error */

    	if(params == NULL || access_ok(VERIFY_WRITE,params,sizeof(struct d_params)) == 0)
        	return -EINVAL;

	/* from the task_struct to d_params */

    	local.deadline_1=task->deadline_1;
    	local.deadline_2=task->deadline_2;
    	local.computation_time=task->computation_time;

    	/* failed to copy */

    	if(copy_to_user(params,&local,sizeof(struct d_params)))
        	return -EFAULT; 

    	/* debug message */

	//printk("get_scheduling_params with PID:%d\n",task->pid);

	printk(KERN_INFO "Name: Ioannis Chatziantoniou , AM: csd5193 , System call: sys_get_scheduling_params\n");

    	return 0;
}

/* sys_get_scheduling_score initialization */

asmlinkage long sys_get_scheduling_score(void){
    	struct task_struct * task=current; /* current process */
    	long d3=get_seconds();   /* time right now */

	printk(KERN_INFO "Name: Ioannis Chatziantoniou , AM: csd5193 , System call: sys_get_scheduling_score\n");

    	/* returns the value of the system */
    	if(d3 <= task->deadline_1)
        	return 100; 
    	else if(d3 > task->deadline_2)
        	return 0; 
    	else if(task->deadline_1 < d3 && d3 <= task->deadline_2)
        	return (task->deadline_2 - d3)*100/(task->deadline_2 - task->deadline_1);
	else
		return -EINVAL;
}


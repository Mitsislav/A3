###########################################
#					  #
#	Ioannis Chatziantwniou		  #
#		csd5193			  #
#					  #
###########################################

inside the scheduling.c file the implementation of all 3 system calls is done. We do at the beginning of the file include d_params.h to add the file that has been defined the struct d_params and then 3 functions follow:

--> for the scheduling.c file

1)asmlinkage long sys_set_scheduling_params(long deadline_1, long deadline_2, long computation_time): it takes the 2 deadlines (long deadline_1, long deadline_2) and the execution time of the current task (something that we will use in exercise 4 and not in this one, it simply does the init). Essentially, it takes the two deadlines and adds to them the current time the machine was booted (because we want the current execution time and not a past time that will be exceeded at some point after the machine boot). Also long current_time=get_seconds(); I use it above the initialization of the struct task fields And then I put in these parameters the current_time together with the offsets (deadline_1 and deadline_2) and I don't do it at that moment because I want exactly the same time in seconds and not with a difference of milliseconds.Then the checks are made that the times are positive and d2>d1 and then our task is inited.
	
2)asmlinkage long sys_get_scheduling_params(struct d_params *params): In this system call we simply have to return the value, that we defined in the previous system call. We need to check that from the user-level the pointer is not NULL and that we have verify-write access because we get from the user-level the pointer * params and we want to write to it the data from the Local variable that is in the kernel space. Then the copy is done and return 0 for success.

3)asmlinkage long sys_get_scheduling_score(void): this function is executed after the end of the process execution. The current time is taken from the machine that was booted and checked, and depending on which case we are in, it returns the appropriate value for score, otherwise -EINVAL.

--> for the file test.c

Here I have defined the macros and the struct and it is to test that the system calls work correctly. Specifically, I call them 12 times and I do this because I have set fixed deadlines d1=5 and d2=15 and I do this because I simulate the process with sleep. If it takes less than 5 seconds then it returns 100 different intermediate values based on the given type and if it is from 15 and above it returns 0 because it has exceeded the limit of d2.

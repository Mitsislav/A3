#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <time.h>

struct d_params {
	long deadline_1;         
    	long deadline_2;         
    	long computation_time;   
};

#define __NR_sys_set_scheduling_params 341
#define __NR_sys_get_scheduling_params 342
#define __NR_sys_get_scheduling_score 343

/* macros */

#define sys_set_scheduling_params(d1, d2, ct) syscall(__NR_sys_set_scheduling_params, d1, d2, ct)
#define sys_get_scheduling_params(params) syscall(__NR_sys_get_scheduling_params, params)
#define sys_get_scheduling_score() syscall(__NR_sys_get_scheduling_score)

int main(){

	int i;

	struct d_params params;

	for(i=4;i<17;i++){

		long d1 = 5;  	/* our deadlines and computation time */
        	long d2 = 15; 
        	long ct = 5;  
		
		if(sys_set_scheduling_params(d1, d2, ct) == -1){	/* setting */
            		perror("sys_set_scheduling_params failed");
            		return -1;
        	}
        	printf("sys_set_scheduling_params succeeded\n");

		if(sys_get_scheduling_params(&params) == -1){	/* getting the already setted params */
            		perror("sys_get_scheduling_params failed");
            		return -1;
        	}
        	printf("sys_get_scheduling_params succeeded with: (%ld %ld %ld)\n",params.deadline_1, params.deadline_2, params.computation_time);
	
		printf("Sleeping time: %d...\n", i);	/* time to finish */
		sleep(i);
		
		long score = sys_get_scheduling_score();	/* calculating the score */
        	if(score == -1){
            		perror("sys_get_scheduling_score failed");
            		return -1;
        	}
        	printf("sys_get_scheduling_score: %ld\n", score);
	}

	return 0;
}

"""
Partitionned EDF using PartitionedScheduler.
"""
from simso.core.Scheduler import SchedulerInfo
from simso.utils import PartitionedScheduler
from simso.schedulers import scheduler

@scheduler("simso.schedulers.P_RM_FIRSTFIT")
class P_RM_FIRSTFIT(PartitionedScheduler):
    def init(self):
        PartitionedScheduler.init(
            self, SchedulerInfo("simso.schedulers.RM_mono"))

    def calc_urm(self, x):
        assert( isinstance(x,int) or isinstance(x,float) )
        return x * ( pow( 2.0, 1.0/x ) - 1.0 )

    def packer(self):
        # First Fit
        cpus = [[cpu, 0] for cpu in self.processors]

        cpus_task_count = [0] * len( self.processors )

        # ensure task list is preprocessed in ascending period values
        self.task_list.sort( key=lambda x: x.period )

        #filt = [ i.period for i in self.task_list ]
        #print ('periods: [%s]' % ', '.join(map(str, filt)) )
                        
        for index_task, task in enumerate(self.task_list):
            j = 0
            for i, c in enumerate(cpus):
                num_tasks = cpus_task_count[i]
                current_cpu_utilization = c[1]
                marginal_utilization = float(task.wcet) / task.period
                urm = self.calc_urm( cpus_task_count[i] + 1 )
                # print( "cpu: " + str(i) + ": urm: " + str(urm) )
                # queue task for cpu if within guaranteed feasibility range
                if current_cpu_utilization + marginal_utilization < urm:
                    j = i
                    cpus_task_count[i] += 1
                    break

            # Affect it to the task.
            self.affect_task_to_processor(task, cpus[j][0])

            # Update utilization.
            cpus[j][1] += float(task.wcet) / task.period

        return True

import torch.multiprocessing as mp
from actor_critic import ActorCritic

from icm import ICM

from shared_adam_opt import AdamOpt
from maze_agent import agent

from p_mqtt_client import MqttClient, threaded_loop_forever

class ParallelEnvironment:
    def __init__(self, env_id, input_shape, n_actions, icm, n_threads=8):
        names = [str(i) for i in range(1, n_threads+1)]

        global_actor_critic = ActorCritic(input_shape, n_actions)
        global_actor_critic.share_memory()
        global_optim = AdamOpt(global_actor_critic.parameters())

        if not icm:
            global_icm = None
            global_icm_optim = None
        else:
            global_icm = ICM(input_shape, n_actions)
            global_icm.share_memory()
            global_icm_optim = AdamOpt(global_icm.parameters())




        #self.client_loop = mp.Process(target=threaded_loop_forever, args=[self.client,])

        self.ps = [mp.Process(target=agent,
                              args=(
                                    name, env_id, input_shape, n_actions,
                                    global_actor_critic, global_icm,
                                    global_optim, global_icm_optim,
                                    n_threads, icm))
                   for name in names]

        [p.start() for p in self.ps]
        #self.client_loop.start()

        [p.join() for p in self.ps]
        #self.client_loop.join()
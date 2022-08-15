import random
import numpy as np
import os

core_folder = 'selected_cores'
sample_size = 10000
for file in os.listdir(core_folder):
    if '.txt' not in file or 'selected' not in file : continue
    name = file.split('.')[-2].split('_')[-1]
    with open(os.path.join(core_folder, file), 'r') as f:
        lins = f.readlines()
    max_alpha = [int(word) for word in lins[0].split()]
    max_beta = [int(word) for word in lins[1].split()]
    assert len(max_alpha) == len(max_beta)
    delta = len(max_alpha)-1
    max_alpha = np.array([c-i for i,c in enumerate(max_alpha)])[1:]
    max_beta = np.array([c-i for i,c in enumerate(max_beta)])[1:]
    max_alpha_p = max_alpha / np.sum(max_alpha)
    max_beta_p = max_beta / np.sum(max_beta)
    alpha_size = int(np.sum(max_alpha) / (np.sum(max_alpha) + np.sum(max_beta)) * sample_size)
    beta_size = sample_size - alpha_size
    alpha_choices = np.random.choice(np.arange(1,delta+1), size=alpha_size, p=max_alpha_p)
    beta_choices = np.random.choice(np.arange(1,delta+1), size=beta_size, p=max_beta_p)

    chosen_pairs = []
    for beta in alpha_choices:
        alpha = random.randint(0, max_alpha[beta]) + beta
        chosen_pairs.append((alpha, beta))
    for alpha in beta_choices:
        beta = random.randint(0, max_beta[alpha]) + alpha
        chosen_pairs.append((alpha, beta))
    assert len(chosen_pairs) == sample_size

    with open(os.path.join(core_folder, f'{name}_cores.txt'), 'w') as f:
        for pair in chosen_pairs:
            f.write(f'{pair[0]} {pair[1]}\n')
    
    
        
    


    

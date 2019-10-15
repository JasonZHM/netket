# Copyright 2018 The Simons Foundation, Inc. - All Rights Reserved.

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#    http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


import netket as nk
from generate_data import generate
import sys
import numpy as np

mpi_rank = nk.MPI.rank()

# Load the data
N = 10
hi, rotations, training_samples, training_bases, ha, psi = generate(
    N, n_basis=2 * N, n_shots=500
)

# Machine
ma = nk.machine.RbmSpinPhase(hilbert=hi, alpha=1)
ma.init_random_parameters(seed=1234, sigma=0.01)

# Sampler
sa = nk.sampler.MetropolisLocal(machine=ma)

# Optimizer
op = nk.optimizer.AdaDelta()

# Quantum State Reconstruction
qst = nk.unsupervised.Qsr(
    sampler=sa,
    optimizer=op,
    n_chains=1000,
    n_samples=1000,
    rotations=rotations,
    samples=training_samples,
    bases=training_bases,
    method="Sr",
)

qst.add_observable(ha, "Energy")


for step in qst.iter(4000, 100):
    obs = qst.get_observable_stats()
    if mpi_rank == 0:
        print("step={}".format(step))
        print("acceptance={}".format(list(sa.acceptance)))
        print("observables={}".format(obs))

        # Compute fidelity with exact state
        psima = ma.to_array()

        fidelity = np.abs(np.vdot(psima, psi))
        print("fidelity={}".format(fidelity))

        # Compute NLL on training data
        nll = qst.nll(
            rotations=rotations,
            samples=training_samples,
            bases=training_bases,
            log_norm=ma.log_norm(),
        )
        print("negative log likelihood={}".format(nll))

        # Print output to the console immediately
        sys.stdout.flush()

        # Save current parameters to file
        ma.save("test.wf")

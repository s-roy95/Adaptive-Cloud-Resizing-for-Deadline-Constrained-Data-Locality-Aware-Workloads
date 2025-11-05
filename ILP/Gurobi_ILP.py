import gurobipy as gp
from gurobipy import GRB
import random

# Problem Parameters
num_data_items = 500  # Data items: 0 to 49
num_jobs = 200       # Number of jobs
items_per_job = 50    # Each job needs 10 data items
deadline = 5       # Maximum completion time
max_items_per_server = 100  # Storage limit per server
max_parallel_access =20    # Max parallel access per server

# Generate random job requirements
random.seed(42)
jobs = {j: random.sample(range(num_data_items), items_per_job) for j in range(num_jobs)}

# Create Gurobi Model
model = gp.Model("Server Minimization")

# Decision Variables
x = model.addVars(1000, num_data_items, vtype=GRB.BINARY, name="x")  # x[s, d] - Store data item d in server s
y = model.addVars(1000, num_jobs, deadline, vtype=GRB.BINARY, name="y")  # y[s, j, t] - Access data on server s at time t
s_used = model.addVars(1000, vtype=GRB.BINARY, name="s_used")  # Whether server is used

# Constraint 1: Each server stores at most 14 unique items
for s in range(1000):
    model.addConstr(gp.quicksum(x[s, d] for d in range(num_data_items)) <= max_items_per_server)

# Constraint 2: Each job must access all its required data within the deadline
for j in range(num_jobs):
    for d in jobs[j]:
        model.addConstr(
            gp.quicksum(y[s, j, t] for s in range(1000) for t in range(deadline) if (s, d) in x) >= 1,
            name=f"job_{j}_data_{d}"
        )

# Constraint 3: Each server can provide at most 2 accesses per time unit
for s in range(1000):
    for t in range(deadline):
        model.addConstr(gp.quicksum(y[s, j, t] for j in range(num_jobs)) <= max_parallel_access)

# Constraint 4: Data items must be stored in a server before accessed
for s in range(1000):
    for j in range(num_jobs):
        for t in range(deadline):
            for d in jobs[j]:
                model.addConstr(y[s, j, t] <= x[s, d])

# Constraint 5: A server is marked used if it stores at least one data item
for s in range(1000):
    model.addConstr(s_used[s] >= gp.quicksum(x[s, d] for d in range(num_data_items)) / max_items_per_server)

# Objective: Minimize the number of servers used
model.setObjective(gp.quicksum(s_used[s] for s in range(1000)), GRB.MINIMIZE)

# Solve the model
model.optimize()

# Print results
print("Optimal number of servers used:", int(sum(s_used[s].x for s in range(1000))))

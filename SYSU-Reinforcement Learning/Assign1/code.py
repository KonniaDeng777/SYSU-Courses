import numpy as np
import matplotlib.pyplot as plt

dim = 8
num_states = []
terminal_state = (6, 8)

maze = np.array([
    [1., 1., 1., 1., 1., 1., 1., 1.],
    [1., 0., 0., 0., 0., 0., 0., 1.],
    [0., 0., 1., 1., 0., 1., 0., 1.],
    [1., 0., 0., 1., 1., 0., 0., 1.],
    [1., 1., 0., 0., 1., 0., 1., 1.],
    [1., 0., 1., 0., 1., 0., 0., 1.],
    [1., 0., 0., 0., 0., 1., 0., 0.],
    [1., 1., 1., 1., 1., 1., 1., 1.]
])

actions = [(-1, 0), (0, 1), (1, 0), (0, -1)]  # Up, Right, Down, Left
directions = ["↑", "→", "↓", "←"]
P_a = np.zeros((dim * dim, 4))
state_map = {}


def init():
    for i in range(dim):
        for j in range(dim):
            if maze[i][j] == 0:
                num_states.append((i, j))
                state_map[(i, j)] = len(num_states) - 1

    for i in range(dim * dim):
        x, y = divmod(i, dim)
        if maze[x][y] == 1:
            continue

        sum_prob = 0
        for k, (dx, dy) in enumerate(actions):
            nx, ny = x + dx, y + dy
            if 0 <= nx < dim and 0 <= ny < dim and maze[nx][ny] == 0:
                P_a[i][k] = 1.
                sum_prob += 1.

        if sum_prob > 0:
            P_a[i] /= sum_prob

    # Handle terminal state manually
    P_a[55][1] = 0.5
    P_a[55][3] = 0.5


def value_iteration(gamma=0.9, num_of_iterations=10000):
    N_STATES = len(num_states)
    N_ACTIONS = len(actions)
    error = 0.0001

    values = np.zeros(N_STATES)
    rewards = [-1] * N_STATES

    for _ in range(num_of_iterations):
        values_tmp = values.copy()

        for idx in range(N_STATES):
            v_a = []
            x, y = num_states[idx]
            s = x * dim + y

            for a in range(N_ACTIONS):
                if P_a[s][a] != 0:
                    nx, ny = x + actions[a][0], y + actions[a][1]
                    if (nx, ny) == terminal_state:
                        v_a.append(P_a[s][a] * (rewards[idx] + gamma * 20000))
                    else:
                        s1 = state_map[(nx, ny)]
                        v_a.append(P_a[s][a] * (rewards[idx] + gamma * values_tmp[s1]))
            values[idx] = max(v_a)

        if max(abs(values - values_tmp)) < error:
            break

    policy = np.zeros(N_STATES)
    for idx in range(N_STATES):
        x, y = num_states[idx]
        s = x * dim + y
        best_action_value = -float('inf')
        best_action = -1

        for a in range(N_ACTIONS):
            if P_a[s][a] != 0:
                nx, ny = x + actions[a][0], y + actions[a][1]
                if (nx, ny) == terminal_state:
                    action_value = P_a[s][a] * (rewards[idx] + gamma * 20000)
                else:
                    s1 = state_map[(nx, ny)]
                    action_value = P_a[s][a] * (rewards[idx] + gamma * values[s1])

                if action_value > best_action_value:
                    best_action_value = action_value
                    best_action = a

        policy[idx] = best_action

    return values, policy


def show(values, type):
    k = 0
    for i in range(dim):
        for j in range(dim):
            if type == "values":
                if maze[i][j] == 1:
                    print("%-5s" % "NAN", end=' ')
                else:
                    print("%-5.2f" % values[k], end=' ')
                    k += 1
            elif type == "policy":
                if maze[i][j] == 1:
                    print("%s" % "■", end=' ')
                else:
                    print(directions[int(values[k])], end=' ')
                    k += 1
        print()


def view(v, type):
    temp = np.full((dim, dim), -9999.0)
    k = 0
    for i in range(dim):
        for j in range(dim):
            if maze[i][j] == 0:
                temp[i][j] = v[k]
                k += 1
    plt.imshow(temp, cmap='Greys_r', interpolation='nearest')
    k = 0
    for i in range(dim):
        for j in range(dim):
            if maze[i][j] == 0:
                if type == "value matrix":
                    plt.text(j, i, f"{temp[i][j]:.2f}", ha='center', va='center', fontsize=7)
                elif type == "policy matrix":
                    plt.text(j, i, directions[int(v[k])], ha='center', va='center', fontsize=14)
                    k += 1
    plt.title(type)
    plt.xticks(())
    plt.yticks(())
    plt.savefig('answer_{}.png'.format(type))
    plt.show()


if __name__ == "__main__":
    init()
    v, p = value_iteration()
    show(v, "values")
    show(p, "policy")
    view(v, "value matrix")
    view(p, "policy matrix")

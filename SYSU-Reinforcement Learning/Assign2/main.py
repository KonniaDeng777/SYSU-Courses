import numpy as np
import matplotlib.pyplot as plt

# Parameters
episodes = 1000
epsilon = 0.05
alpha = 0.4
gamma = 0.99
rows, cols = 4, 12
directions = 4


def initialize_q_table():
    return np.zeros((rows * cols, directions))


def max_q_value(Q_table, pos):
    idx = pos[0] * cols + pos[1]
    return np.max(Q_table[idx])


def epsilon_greedy(Q_table, pos):
    if np.random.random() < epsilon:
        return np.random.randint(directions)
    return np.argmax(Q_table[pos[0] * cols + pos[1]])


def move(pos, action):
    x, y = pos
    if action == 0 and x > 0:
        x -= 1
    elif action == 1 and x < rows - 1:
        x += 1
    elif action == 2 and y > 0:
        y -= 1
    elif action == 3 and y < cols - 1:
        y += 1
    return (x, y)


def calculate_reward(new_pos):
    end = (new_pos == (3, 11))
    if new_pos[0] == 3 and 1 <= new_pos[1] <= 10:
        return (3, 0), -100, False
    return new_pos, -1, end


def q_learning():
    Q_table = initialize_q_table()
    clif_map = np.zeros((rows, cols))
    reward_list = []

    for _ in range(episodes):
        pos, total_reward = (3, 0), 0
        end = False
        clif_map.fill(0)

        while not end:
            clif_map[pos[0], pos[1]] = 1
            action = epsilon_greedy(Q_table, pos)
            next_pos = move(pos, action)
            next_pos, reward, end = calculate_reward(next_pos)

            max_q_next = max_q_value(Q_table, next_pos)
            idx = pos[0] * cols + pos[1]
            Q_table[idx, action] += alpha * (reward + gamma * max_q_next - Q_table[idx, action])

            pos = next_pos
            total_reward += reward

        reward_list.append(total_reward)
    return clif_map, Q_table, reward_list


def sarsa():
    Q_table = initialize_q_table()
    clif_map = np.zeros((rows, cols))  # Track path taken in SARSA
    reward_list = []

    for _ in range(episodes):
        pos, total_reward = (3, 0), 0
        end = False
        clif_map.fill(0)  # Reset map for each episode
        action = epsilon_greedy(Q_table, pos)

        while not end:
            clif_map[pos[0], pos[1]] = 1  # Mark visited positions
            next_pos = move(pos, action)
            next_pos, reward, end = calculate_reward(next_pos)

            next_action = epsilon_greedy(Q_table, next_pos)
            idx = pos[0] * cols + pos[1]
            Q_table[idx, action] += alpha * (
                        reward + gamma * Q_table[next_pos[0] * cols + next_pos[1], next_action] - Q_table[idx, action])

            pos, action = next_pos, next_action
            total_reward += reward

        reward_list.append(total_reward)
    return clif_map, Q_table, reward_list


def show_results(path_q, path_s, q_rewards, sarsa_rewards):
    path_q[3, 1:11] = -0.1  # Mark cliff in Q-learning path
    path_s[3, 1:11] = -0.1  # Mark cliff in SARSA path

    for i in range(rows):
        for j in range(cols):
            if path_q[i, j] == 1:
                path_q[i, j] = 0.05
            if path_s[i, j] == 1:
                path_s[i, j] = 0.05

    plt.figure(figsize=(10, 6))
    plt.subplot(2, 1, 1)
    plt.title("Q-learning path")
    plt.axis('off')
    plt.imshow(path_q, cmap='cividis', interpolation='nearest')

    plt.subplot(2, 1, 2)
    plt.title("SARSA path")
    plt.axis('off')
    plt.imshow(path_s, cmap='cividis', interpolation='nearest')

    plt.savefig('path.png')
    plt.show()

    plt.figure(figsize=(20,6))
    plt.plot(range(episodes), q_rewards, label="Q-learning")
    plt.plot(range(episodes), sarsa_rewards, label="SARSA")
    plt.xlabel("Episodes")
    plt.ylabel("Cumulative Reward")
    plt.legend()

    plt.savefig('output.png')
    plt.show()


if __name__ == "__main__":
    print("Starting Q-learning and SARSA...")
    path_q, q_table, q_rewards = q_learning()
    path_s, sarsa_table, sarsa_rewards = sarsa()

    print("Q-learning Q-table:\n", q_table)
    print("SARSA Q-table:\n", sarsa_table)

    show_results(path_q, path_s, q_rewards, sarsa_rewards)
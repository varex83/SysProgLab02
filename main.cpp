#include <iostream>
#include <vector>
#include <queue>

class FiniteAutomata {
protected:
    int alaphabet_size;
    int state_size;
    int initial_state;
    int final_state_size;

    std::vector<int> final_states;
    std::vector<std::vector<int>> transition_table;
    std::vector<std::vector<int>> inv_transition_table;

    std::vector<int> is_state_possibly_final;

    void clear_and_init_transition_table() {
        transition_table.clear();
        inv_transition_table.clear();
        for (int i = 0; i < state_size; i++) {
            std::vector<int> transition;
            for (int j = 0; j < alaphabet_size; j++) {
                transition.push_back(-1);
            }
            transition_table.push_back(transition);
            inv_transition_table.push_back(transition);
        }
    }
public:
    FiniteAutomata(FILE* file) {
        if (file == NULL) {
            std::cout << "File not found!" << std::endl;
            return;
        }

        fscanf(file, "%d", &alaphabet_size);
        fscanf(file, "%d", &state_size);
        fscanf(file, "%d", &initial_state);
        fscanf(file, "%d", &final_state_size);

        is_state_possibly_final = std::vector<int>(state_size, 0);

        for (int i = 0; i < final_state_size; i++) {
            int final_state;
            fscanf(file, "%d", &final_state);
            final_states.push_back(final_state);
        }

        clear_and_init_transition_table();

        int s, sp, a;
        while (fscanf(file, "%d %d %d", &s, &a, &sp) != EOF) {
            transition_table[s][a] = sp;
            inv_transition_table[sp][a] = s;
        }

        std::queue<int> q;

        for (int i = 0; i < final_states.size(); i++) {
            q.push(final_states[i]);
            is_state_possibly_final[final_states[i]] = 1;
        }

        while (!q.empty()) {
            int current_state = q.front();
            q.pop();
            for (int i = 0; i < alaphabet_size; i++) {
                int previous_state = inv_transition_table[current_state][i];
                if (previous_state != -1 && is_state_possibly_final[previous_state] == 0) {
                    q.push(previous_state);
                    is_state_possibly_final[previous_state] = 1;
                }
            }
        }
    }

    FiniteAutomata(
            int alaphabet_size,
            int state_size,
            int initial_state,
            int final_state_size,
            std::vector<int> final_states,
            std::vector<std::vector<int>> transition_table
    ) {
        this->alaphabet_size = alaphabet_size;
        this->state_size = state_size;
        this->initial_state = initial_state;
        this->final_state_size = final_state_size;
        this->final_states = final_states;
        this->transition_table = transition_table;
    }

    bool is_accepted(std::string input) {
        int current_state = initial_state;
        for (int i = 0; i < input.size(); i++) {
            int input_index = input[i] - 'a';
            int old_state = current_state;
            current_state = transition_table[current_state][input_index];

            if (current_state == -1) {
                return false;
            }
        }
        for (int i = 0; i < final_states.size(); i++) {
            if (current_state == final_states[i]) {
                return true;
            }
        }
        return false;
    }

    bool is_word_a_prefix(std::string input) {
        int current_state = initial_state;
        for (int i = 0; i < input.size(); i++) {
            int input_index = input[i] - 'a';
            int old_state = current_state;
            current_state = transition_table[current_state][input_index];

            if (current_state == -1) {
                return false;
            }
        }

        if (is_state_possibly_final[current_state] == 0) {
            return false;
        }

        return true;
    }

    ~FiniteAutomata() {
        final_states.clear();
        transition_table.clear();
    }
};

int main() {
    FILE* file = fopen("input.txt", "r");

    FiniteAutomata finite_automata(file);

    fclose(file);

    std::string input;

    std::cout << "Input: ";

    std::cin >> input;

    std::cout << std::endl;

    if (finite_automata.is_accepted(input)) {
        std::cout << "Accepted!" << std::endl;
    } else {
        std::cout << "Not accepted!" << std::endl;
    }

    if (finite_automata.is_word_a_prefix(input)) {
        std::cout << "Is a prefix!" << std::endl;
    } else {
        std::cout << "Is not a prefix!" << std::endl;
    }

    return 0;
}
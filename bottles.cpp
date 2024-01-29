/*
* there are games that involve several bottles or containers filled with different colored liquids, 
* and the goal is to separate each color into its own designated bottle using a series of moves or manipulations
* this algorithm implement a recursive function to find solution(s) for such puzzles
* we represent bottle as a vector of integers, where the first element is the upper part of it and the whole puzzle
* as a vector of such vectors
*/

#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;

bool bottle_is_empty(vi bottle) {
    return bottle[bottle.size() - 1] == 0;
}

bool bottle_is_full(vi bottle) {
    return bottle[0] != 0;
}

int top_colour(vi bottle) {
    if (bottle_is_empty(bottle)) {
        return 0;
    }
    else {
        int cnt = 0;
        int element = bottle[cnt];
        while (element == 0) {
            cnt++;
            element = bottle[cnt];
        }
        return element;
    }
}

int top_colour_volume(vi bottle) {
    if (bottle_is_empty(bottle)) {
        return 0;
    }
    else {
        int top_col = top_colour(bottle);
        int cnt = 0;
        int volume = 0;
        while ((bottle[cnt] == 0 || bottle[cnt] == top_col) && (cnt < bottle.size() - 1)) {
            if (bottle[cnt] == top_col)
                volume++;
            cnt++;
        }
        if (bottle[cnt] == top_col)
            volume++;
        return volume;
    }
}

int capacity(vi bottle) {
    if (bottle_is_full(bottle))
        return 0;
    else if (bottle_is_empty(bottle))
        return bottle.size();
    else {
        int cap = 0;
        while (bottle[cap] == 0)
            cap++;
        return cap;
    }
}

bool move_is_valid(vvi current_state, int from, int to) {
    vi from_bottle = current_state[from];
    vi to_bottle = current_state[to];
    if ((!bottle_is_empty(from_bottle)) && bottle_is_empty(to_bottle)) {
        return true;
    }
    else if ((!bottle_is_empty(from_bottle)) && (top_colour(from_bottle) == top_colour(to_bottle)) && capacity(to_bottle) > 0) {
        return true;
    }
    else {
        return false;
    }
}

bool solved(vvi state) {
    for (vi bottle : state) {
        int top = bottle[0];
        for (int i = 1; i < bottle.size(); i++) {
            if (bottle[i] != top) {
                return false;
            }
        }
    }
    return true;
}

vvi make_move(vvi state, int from, int to) {
    vvi new_state = state;
    if (move_is_valid(new_state, from, to)) {
        int volume_from = top_colour_volume(new_state[from]);
        int capacity_to = capacity(new_state[to]);
        if (volume_from <= capacity_to) {
            int capacity_from = capacity(new_state[from]);
            int top_colour_from = top_colour(new_state[from]);
            for (int i = 0; i < volume_from; i++) {
                new_state[from][capacity_from + i] = 0;
                new_state[to][capacity_to - 1 - i] = top_colour_from;
            }
        }
        else {
            int capacity_from = capacity(new_state[from]);
            int top_colour_from = top_colour(new_state[from]);
            for (int i = 0; i < capacity_to; i++) {
                new_state[from][capacity_from + i] = 0;
                new_state[to][capacity_to - 1 - i] = top_colour_from;
            }
        }
    }
    return new_state;
}

void solve(vvi state, vvi moves = {}, vi prev_move = {-1, -1}) {
    vvi moves_copy = moves;
    if (solved(state)) {
        printf("solved!\n");
        for (vi move : moves_copy) {
            for (int x : move) {
                printf("%d ", x);
            }
            printf("\n");
        }
    }
    else {
        for (int i = 0; i < state.size(); i++) {
            for (int j = 0; j < state.size(); j++) {
                if (solved(state)) {
                    break;
                }
                if (i != j) {
                    if ((prev_move[0] != j) && (prev_move[1] != i)) {
                        if (move_is_valid(state, i, j)) {
                            moves_copy.push_back({ i, j });
                            //printf("depth %d\n", moves_copy.size()); // to track the depth of recursion 
                            state = make_move(state, i, j);
                            solve(state, moves_copy, { i, j });
                        }
                    }  
                }
            }
        }
    }
}



int main(){
    vvi test = {
        {1,1,2,2},
        {2,2,1,1},
        {0,0,0,0}
    };
    solve(test);
    return 0;
}
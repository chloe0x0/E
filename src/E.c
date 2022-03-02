#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

struct Universe{ bool* state; uint8_t rule; unsigned int length; };

struct Universe* ConstructUniverse(unsigned int len, uint8_t rule){
    struct Universe* u = malloc( sizeof(struct Universe) );
    u->state = calloc(len, sizeof(bool));
    u->rule = rule;
    u->length = len;

    return u;
}

// Function to free a Universe struct from memory
void FreeUniverse(struct Universe* universe){
    free(universe->state);
    free(universe);
}

// Randomly sets the cell states
void RandomState(struct Universe* u){
    for (int i = 0; i < u->length; ++i){
        u->state[i] = rand() % 2;
    }
}

// Sets only the center cell to true
void CenterState(struct Universe* u){
    u->state[u->length / 2] = 1;
}

// evolve the state of the system by one epoch according to its rule
void Evolve(struct Universe* universe){
    int ruleIndex;
    bool* newState = calloc(universe->length, sizeof(bool));

    for (int i = 1; i < universe->length; ++i){
        bool l, c, r;
        l = universe->state[i - 1];
        c = universe->state[i];
        r = universe->state[i + 1];

        ruleIndex = (l*4) + (c*2) + (r);        
        newState[i] = universe->rule & (1 << ruleIndex);
        ruleIndex = 0;
    }
    
    free(universe->state);
    universe->state = newState;
}

char* Universe2String(struct Universe* universe, bool displayEmpty){
    char* str = malloc( sizeof(char) * universe->length+1 );
    const char empty = displayEmpty ? '0' : ' '; 

    for (int i = 0; i < universe->length; ++i){ str[i] = universe->state[i] ? '1' : empty; }
    str[universe->length + 1] = '\0';

    return str;
}


// E.exe <Length> <Rule> <Epochs> <Centered>
int main(int argc, char** argv){
    if (argc < 5){
        fprintf(stderr, "Insufficient flags provided!\n");
        exit(1);
    }

    unsigned int Length = atoi(argv[1]);
    uint8_t Rule = atoi(argv[2]);
    unsigned int Epochs = atoi(argv[3]);
    bool centered = atoi(argv[4]);

    srand(time(NULL));

    struct Universe* u = ConstructUniverse(Length, Rule);

    if (centered){ CenterState(u); }
    else { RandomState(u); }

    printf("%s\n", Universe2String(u, false));
    for (int i = 0; i < Epochs; ++i){
        Evolve(u);
        printf("%s\n", Universe2String(u, false));
    }
}
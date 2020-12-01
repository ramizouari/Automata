#pragma once
#include "epsilon_non_deterministic_finite_state_automata.h"

/*
* This enumeration is used to mark special values for some charaters
* - any: .
* - word: \w
* - numeric: \d
* - left_group: (
* - right_group: )
* - disjunction: |
* - kleene_star: *
* - kleene_plus: +
* - kleene_quest: ?
* - left_class: [
* - right_class: ]
* - class_dash: -
*/
enum character_class
{
    any=-2,word=-3,numeric=-4,alpha=-5,upcase=-6,downcase=-7,left_group=1,right_group,
    disjunction,kleene_star,kleene_plus,kleene_quest,left_class,right_class,class_dash
};

class regular_expression_automata :
    public epsilon_non_deterministic_finite_state_automata

{
    int intervals_number;
    std::map<int, std::pair<int, int>> intervals;
public:
    using epsilon_non_deterministic_finite_state_automata::epsilon_non_deterministic_finite_state_automata;
    bool execute(std::deque<int>& S) const override;
    void add_transition(int i, int j, int a, int b);
    void add_transition(int i, int j, character_class c);
    using epsilon_non_deterministic_finite_state_automata::add_transition;
};


#pragma once
#include "fsm/epsilon_non_deterministic_finite_state_automata.h"
#include <locale>

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
* - beginning: ^
* - end: $
*/
enum character_class
{
    any,word,numeric,alpha,upcase,downcase,left_group,right_group,
    disjunction,kleene_star,kleene_plus,kleene_quest,left_class,right_class,class_dash,begin,end
};

/*
* This is a class of regular expression automatas (parsers): REA
* It is a generalisation of NFA_ε with introduction of character classes
* A character is a label
* A character class is a set of characters
* A transition from u to v consisting of a character class C is informally the same as the set of transitions
* from u to v consisting of characters e in C

*/

class regular_expression_automata :
    public epsilon_non_deterministic_finite_state_automata

{
    int intervals_number;
    std::map<int, std::pair<int, int>> intervals;
    std::locale loc;
public:
    regular_expression_automata(const std::locale & L,int n = 0, int q0 = 0, const std::set<int>& F = {});
    regular_expression_automata(int n = 0, int q0 = 0, const std::set<int>& F = {});
    bool execute(std::deque<int>& S) const override;
    void add_transition(int i, int j, int a, int b);
    void add_transition(int i, int j, character_class c);
    using epsilon_non_deterministic_finite_state_automata::add_transition;
};


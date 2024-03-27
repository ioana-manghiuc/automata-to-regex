# automata-to-regex
Converting a Finite Automata to a Regular Expression using the State Elimination Method.

How it works:
- input is read from the "automata.txt" file and stored in a FiniteAutomata object
- the automata is made uniform (adds one initial state and one final state, connected to the previous initial and final states through lambda transitions)
- for multiple transitions existing between state1 and state2, they are merged into a single transition having a union of the symbols
  (eg. (state1,a) = state2 and (state1,b) = state2 => (state1, (a|b)) = state2)
  
- while the automata has more than two states:
   1. choose a random non-endpoint state k
   2. find all paths connected via k (p->k->q, where p,q !=k)
   3. compute regex: label = label(pq)|label(pk)(label(kk))*label(kq)
   4. create transition (p, label) = q (or set the label if it already exists)
   5. remove state k and all transitions that contain it
- at the end, only one transition will remain between the initial and final state, and its label will be the equivalent regular expression

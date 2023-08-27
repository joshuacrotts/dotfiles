1930s
Lambda calculs
syntax e ::= x | lam x e | e e
Rule (lam x e)e' = e[e'/x]

let x = 3
    f = lam y x
    x = 4
in f 5
    3 in lex scope
    4 in dym scope

- scope 
- types
- call by value/name
  e.g., f(x) = 5
        what about f(1/0)? -- is it 5? is it an error?
        most languages before haskell chose the latter; eval all arguments in cbv
        


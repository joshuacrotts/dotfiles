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
        f(x) = x + x + x + ...
        f(300!) = ... wasting time if it's not necessary
        haskell copies 300! to get f(x) = 300! + 300! + 300! + 300!
        this is inefficient--why not cache 300! and use call-by-need?
            reuse the result!
        this is only safe if we can't update the result
            in haskell's model, it's called lazy evaluation and proved to be 
            impossible to observe the difference, i.e., indistinguishable in lam-calculus
    Ex:
    y = 5
    f(x) = x + x + y
    What is f(++y)?
    CBV: 6 + 6 + 6 = 18
    CBName: (++y) + (++y) + y
            = 6 + 7 + 7 = 20
    CBNeed: = 6 + 6 + 6 = 18... but in Haskell this is impossible. 
                                In general they are NOT The same!
        
haskell has static scoping, types, and lazy
problem: the order of evaluation is impossible to predict
    what if you put a print statement? it will print it only when the variable
        is referenced. e.g., f(print(300!)) = print(300!) + cached result + cached result...

        


# Functions by pattern matching

We break the incoming type into cases. We're going to break it into zero and... non-zero.
Tried sequentially.
`0` and `n` are simple patterns.

```Haskell
fact :: Int -> Int
fact 0 = 1
fact n = n * fact (n - 1)
```

With only natural numbers, it's hard to see the point. Let's try lists.
`[]` is the empty list
`(x : xs)` is like matching `(quasiquote (,a . ,d))`. `x` is like the `car`, and `xs` is like the `cdr`.

```Haskell
sum :: [Int] -> Int
sum [] = 0
sum (x : xs) = x + sum xs
```

We know that `[1, 2, 3]` is abbreviation of `1 : (2 : (3 : []))`

We could, for example, extract the third element of a list via the pattern `(_ : _ : x : _)`. If there is no third, an error is thrown.

Let's write `prod`, which has a third case for when the first element is zero. Let's pattern match against this.

```Haskell
prod :: [Int] -> Int
prod [] = 1
prod (0 : xs) = 0
prod (x : xs) = x * prod xs
```

Writing pattern cases need to be exhaustive and non-overlapping. Omitting the former can result in a runtime error.


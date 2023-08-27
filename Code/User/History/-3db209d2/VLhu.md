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
sum []       = 0
sum (x : xs) = x + sum xs
```

We know that `[1, 2, 3]` is abbreviation of `1 : (2 : (3 : []))`

We could, for example, extract the third element of a list via the pattern `(_ : _ : x : _)`. If there is no third, an error is thrown.

Let's write `prod`, which has a third case for when the first element is zero. Let's pattern match against this.

```Haskell
prod :: [Int] -> Int
prod []       = 1
prod (0 : xs) = 0
prod (x : xs) = x * prod xs
```

Writing pattern cases need to be exhaustive and non-overlapping. Omitting the former can result in a runtime error.

# Polymorphic Types
Functions that work for arbitrary types.

E.g., `map`; it receives two arguments: a function from type `a` to `b`, a list of `a`, and returns a list of `b`. Think of this as $\forall a, b$, where $a$ and $b$ are types.

We *cannot* match on functions; they're opaque objects and infinite.

```Haskell
map :: (a -> b) -> [a] -> [b]
-- If the list is empty, just return it.
map f [] = []
-- We use 'a' since it has the same identifier as the type.
map f (a : as) = (f a) : (map f xs)
```

All functions in Haskell are curried, which is why we add an arrow in between the first and second parameters. We could do the following:

```Haskell
map2 :: (a -> b, [a]) -> [b]
map2 (f, [])     = []
map2 (f, a : as) = (f a) : map2 (f, as)
```
 
The issue is that we **have** to call it with two arguments.

Let's show an invocation of `map`:

```Haskell
-- even :: (Int -> Bool), so we get [true, false, true, false]
map even [0, 1, 2, 3]
```

Applying a function is left-associative, i.e., $((f\;a)\;b)\;c$, but defining a function is right-associative, i.e., $f : a \to (b \to c)$.

Let's rewrite the remove duplicates function. We want to have the "rest" of the list contain no "a" elements.

```Haskell
removeDups :: Eq a => [a] -> [a]
removeDups []       = []
removeDups (a : as) = a : removeDups (filter (/= a) as)
```

We have to know if two things of type `a` are equal, which means that our signature needs to have a way of equality on `a`. That's why we use `Eq a`. Many things we can't check for equality, e.g., functions, streams, ..., but we can qualify the types as we just showed.

Let's now write `filter`. To do this, we need to perform a case analysis on `f a`. We could also use `if f a then ...`, but this approach is more idiomatic.

```Haskell
filter : (a -> Bool) -> [a] -> [a]
filter f []       = []
filter f (a : as) | f a       = a : filter f as
                  | otherwise = filter f as
```

Let's write another version of `removeDups`. We perform a case analysis on whether `a` is a member of the rest of the list. If so, we throw away the first and recurse on the rest.

```Haskell
removeDups2 :: [a] -> [a]
removeDups2 [] = []
removeDups2 (a : as) | a `elem` as = removeDups2 as
                     | otherwise   = a : removeDups2 as
```

The `elem` function has type `a -> [a] -> Bool`, and can be called, e.g., `elem a as`, but to make it infix, we use the backticks, i.e., `a \`elem\` as`.

Both of these are $O(n^2)$ algorithms, though! Let's do better by checking to see 
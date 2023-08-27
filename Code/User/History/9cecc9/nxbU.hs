removeDups :: Ord a => [a] -> [a]
removeDups = map head . group . sort
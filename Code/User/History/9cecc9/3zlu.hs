import Prelude hiding (head, group, sort)

removeDups :: Ord a => [a] -> [a]
removeDups = map head . group . sort
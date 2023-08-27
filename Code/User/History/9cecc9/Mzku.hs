import Prelude hiding (head, group, sort)
import Data.List

removeDups :: Ord a => [a] -> [a]
removeDups [] = []
removeDups [a] = [a]
removeDups as = 
    let sas = sort as
    in
        removeDups()

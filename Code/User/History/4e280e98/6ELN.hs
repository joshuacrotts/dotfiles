module TestHomework0 (testHomework0) where

import           Test.Tasty
import qualified Test.Tasty.QuickCheck as QC
import           Test.Tasty.HUnit ( Assertion, testCase, (@?=) )

import           Homework0

--------------------------------------------------------------------------------

testHomework0 :: TestTree
testHomework0 = testGroup "Homework0 tests" [qcProps, unitTests]

qcProps :: TestTree
qcProps = testGroup "(checked by QuickCheck)"
  [ QC.testProperty "forall x, x + 1 > x" prop_greaterPlusOne
  , QC.testProperty "forall x, x /= x (failing)" prop_equiv]
  
unitTests :: TestTree
unitTests = testGroup "Unit tests"
  [ testCase "-1 + 1 == 0" ut_passing
  , testCase "5 + 1 == 6 (failing)" ut_failing
  ]


prop_greaterPlusOne :: Int -> Bool
prop_greaterPlusOne x = (x + 1) > (x)


prop_equiv :: Int -> Bool
prop_equiv x = x /= x


ut_passing :: Assertion
ut_passing = plusOne (-1) @?= 0

ut_failing :: Assertion
ut_failing = plusOne 5 @?= 7


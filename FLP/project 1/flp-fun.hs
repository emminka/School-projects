--FLP Projekt 1 - Haskell
--Emma Krompascikova
--xkromp00
--2023/2024

import System.Environment 
import Data.List.Split
import Data.Ord
import Data.Map (fromListWith, toList)
import Data.Function
import Data.List


data Tree = Leaf String 
          | Node Int Float Tree Tree 

instance Show Tree where
    show t = showTree t 0

showTree :: Tree -> Int -> String
showTree (Leaf s) indentation = take indentation (repeat ' ') ++ "Leaf: " ++ s ++ "\n"
showTree (Node index threshold left right) indentation = take indentation (repeat ' ') ++ "Node: " ++ show index ++ ", " ++ show threshold ++ "\n" ++ showTree left (indentation + 2) ++ showTree right (indentation + 2)

--FOR PART 1 OF PROJECT

removeNewlinesAndEmpty :: [String] -> [String]
removeNewlinesAndEmpty = map (filter (\c -> c /= '\n' && c /= ',')) . filter (not . null)


--functions for converting input string to tree
cleanedListToTree :: [String] -> Tree
cleanedListToTree xs =
    case stringToList xs of
        [(tree, [])] -> tree
        _ -> error "Invalid input" 

stringToNode :: [String] -> Maybe (Tree, [String])
stringToNode ("Leaf:" : trida : rest) = Just (Leaf trida, rest)
stringToNode ("Node:" : indexFeature : threshold : rest) =
    case stringToList rest of
        (leftSon, _) : (rightSon, rest2) : _ -> Just (Node (read indexFeature) (read threshold) leftSon rightSon, rest2)
        _ -> Nothing
stringToNode _ = Nothing

stringToList :: [String] -> [(Tree, [String])]
stringToList [] = []
stringToList xs =
    case stringToNode xs of
        Just (tree, remaining) -> (tree, remaining) : stringToList remaining
        Nothing -> error "Invalid input"



--splitting on newlines and commas
makeListFromNewData :: String -> [[String]] 
makeListFromNewData input = fmap (splitOn ",") (lines input)

--convert 2d array of strings to floats
makeFloats :: [[String]] -> [[Float]]
makeFloats final = fmap (\oneLine -> fmap read oneLine) final

--making inference for all samples
makeFinalDecision :: Tree -> [[Float]] -> [String]
makeFinalDecision strom newdata = fmap (\oneLine -> makeFinalDecisionSingle strom oneLine) newdata

--making inference on one sample
makeFinalDecisionSingle :: Tree -> [Float] -> String
makeFinalDecisionSingle (Leaf x) _ = x
makeFinalDecisionSingle (Node indexFeature threshold leftSon rightSon) newdata = 
    if threshold >= newdata !! indexFeature then makeFinalDecisionSingle leftSon newdata
    else makeFinalDecisionSingle rightSon newdata

-----------------------------------------------------------------------------------------
-- FOR PART 2 OF PROJECT

--making from input training data as a tuple
inputSecondPart :: String -> [([Float], String)] 
inputSecondPart vstup =  map makeTuple (lines vstup)
    

makeTuple :: String -> ([Float], String) 
makeTuple onlyOneLine = 
    let cast =  splitOn "," onlyOneLine
    in (init (map read cast), (last cast))
--init takes everything but last , in last is the class name

countOccurrences :: Ord a => [a] -> [(a, Int)]
countOccurrences = toList . fromListWith (+) . map (\x -> (x, 1))

giniIndexLeaf :: [([Float], String)] -> Float
giniIndexLeaf combinedTuple = 
    let
        modifiedTuple = countOccurrences $ fmap snd combinedTuple
        sumOfAll = fromIntegral (sumNumbers modifiedTuple);
        particularNumbers = fmap ((/ sumOfAll) . fromIntegral . snd) modifiedTuple
    in 1.0 - sum (fmap (\x -> x * x) particularNumbers)

--gives me the sum of all elements (tridaa,tridab) ->2
sumNumbers :: [(String, Int)] -> Int 
sumNumbers combinedTuple = foldl (+) 0 $ map snd combinedTuple

giniIndex :: Tree -> [([Float], String)] -> Float
giniIndex (Leaf _) trainingData = giniIndexLeaf trainingData
giniIndex (Node index threshold leftSon rightSon) trainingData = let
    lenOfTrainData = fromIntegral (length trainingData) ;
    leftData = filter (\oneLine -> ((fst oneLine) !! index) <= threshold ) trainingData;
    rightData = filter (\oneLine -> ((fst oneLine) !! index) > threshold ) trainingData;
    leftGiniIndex = giniIndex leftSon leftData;
    rightGiniIndex = giniIndex rightSon rightData
    in (((fromIntegral (length leftData)) / lenOfTrainData) * leftGiniIndex) + (((fromIntegral (length rightData)) / lenOfTrainData) * rightGiniIndex)

--returns list of all possible trees received from a single split
allSplittingsAvailable :: Tree -> [([Float], String)] -> [Tree]
allSplittingsAvailable _ [] = []
allSplittingsAvailable (Leaf _) trainingData =
    let
        cleanedData = map fst trainingData
    in
        [
            Node indexFeature ((cleanedData !! indexRow) !! indexFeature) (Leaf "") (Leaf "") |
                indexFeature <- [0 .. (length (cleanedData !! 0)) - 1],
                indexRow <- [0 .. (length cleanedData) - 1 ]
        ]
allSplittingsAvailable (Node index threshold leftSon rightSon) trainingData = 
    let
        leftData = filter (\oneLine -> ((fst oneLine) !! index) <= threshold ) trainingData;
        rightData = filter (\oneLine -> ((fst oneLine) !! index) > threshold ) trainingData;
    in (fmap (\x -> Node index threshold x rightSon) (allSplittingsAvailable leftSon leftData)) ++ (fmap (\x -> Node index threshold leftSon x) (allSplittingsAvailable rightSon rightData))

-- returns the best tree (lowest Gini index)
trainTree :: Tree -> [([Float], String)] -> Tree
trainTree tree tuple =
    let
        previousGiniIndex = giniIndex tree tuple;
        allSplits = (allSplittingsAvailable tree tuple);
        allSplitsGini = fmap (\x -> giniIndex x tuple) allSplits;
        bestSplit = minimumBy (comparing snd) (zip allSplits allSplitsGini);
        newTree = fst bestSplit
        newGiniIndex = snd bestSplit
    in if newGiniIndex < previousGiniIndex then trainTree newTree tuple else tree

--fills in the unnamed Leaf CLASS based on majority from training data
nameLeaves :: Tree -> [([Float], String)] -> Tree
nameLeaves (Leaf _) tuple = Leaf (mostFrequentInString (fmap snd tuple))
nameLeaves (Node indexFeature threshold leftSon rightSon) tuple = 
    let
        leftData = filter (\oneLine -> ((fst oneLine) !! indexFeature) <= threshold ) tuple;
        rightData = filter (\oneLine -> ((fst oneLine) !! indexFeature) > threshold ) tuple;
    in (Node indexFeature threshold (nameLeaves (leftSon) leftData)  (nameLeaves(rightSon)rightData))

mostFrequentInString :: [String] -> String
mostFrequentInString = head . maximumBy (compare `on` length) . group . sort
-----------------------------------------------------------

main :: IO ()
main  = do
    inputArguments <- getArgs   
    let firstArgument = head inputArguments
    -- FOR PART 1 OF PROJECT
    if firstArgument == ("-1") then do
        --taking second argument, its the name of file consisting of tree
        let fileTree = inputArguments !! 1 
        contentFileTree <- readFile fileTree

        let splitted  = splitOn (" ") contentFileTree
        
        --string which has everything from file in a line like
        --["Node:","1","3.0","Leaf:","TridaB","Leaf:","TridaC"]
        let cleanedList = removeNewlinesAndEmpty splitted

        --parsing list to the data strecture Tree
        let parsedTree = cleanedListToTree cleanedList
        
        --taking third argument, its the name of file consisting of newdata
        let fileNewData = inputArguments !! 2
        contentFileNewData <- readFile fileNewData

        let newDataString = makeListFromNewData contentFileNewData

        let finalString = makeFloats newDataString
       
        --final print for the first part of project
        let finishedClasses = makeFinalDecision parsedTree finalString
        putStr ( unlines  finishedClasses)
    -- FOR PART 2 OF PROJECT
    else
        if firstArgument == ("-2") then do
            --taking second argument, its the name of file consisting of training data
            let fileTrainData = inputArguments !! 1 
            contentFileTrainData <- readFile fileTrainData
            
            let tupleTrData = inputSecondPart contentFileTrainData
            let root = Leaf ""
            let bestTree = trainTree root tupleTrData
            let finalNameLeaves = nameLeaves bestTree tupleTrData
            
            putStr . show $ finalNameLeaves

        else
            --we have to get as an first argument only '-1' or '-2'
            print("Invalid argument.")

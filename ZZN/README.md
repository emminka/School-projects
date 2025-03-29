## Knowledge Discovery in Databases - Australian Weather Data Mining Project - Team project

## Project Overview
Analysis of 145,460 weather records from 49 Australian locations (2007-2017) using data mining techniques.

## Project Parts

### Part 1: Data Analysis Proposal (`part1.pdf`)
- **Dataset**: 49 locations across Australia
- **Key Features**: 
  - Temperature, Rainfall, Humidity, Pressure measurements
  - 10 years of daily records (2007-2017)
- **Challenges**:
  - High missing values (38%+ in 4 attributes)
  - Geographic diversity across climate zones

### Part 2: Implementation (`part2.pdf`)
#### Key Analyses
1. **Climate Zone Clustering (K-means)**
   - 7 climate zones identified (Alpine, Desert, Tropical etc.)
   - Achieved 59.13% accuracy for Tropical zone
   - Tools: RapidMiner K-means (k=7, 10 runs)

2. **Rainfall Prediction**
   - Best model: **Deep Learning (84.16% accuracy)**
   - Compared 7 machine learning algorithms
   - Handled class imbalance (77.6% "No Rain" instances)

3. **Weather Pattern Mining (FP-Growth)**
   - Discovered 50+ association rules
   - Minimum confidence threshold: 0.7

## Technical Implementation
- **Data Processing**:
  - Created new "Climate" zone attribute
  - Cleansed 145k+ records
  - Multiple imputation strategies tested
- **Tools Used**:
  - RapidMiner

## 🔗 Dataset Source
[Kaggle Weather Dataset](https://www.kaggle.com/datasets/jsphyg/weather-dataset-rattle-package)
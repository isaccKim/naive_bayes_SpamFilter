# README: Spam Email Classification Program

## Overview
This program implements a spam email classification system using a simple frequency-based method. It reads email datasets, processes them to extract words, calculates spam and ham probabilities, and evaluates test emails against a trained model to classify them as spam or non-spam (ham).

## Features
- Preprocesses email text by extracting words and removing non-alphabetic characters.
- Calculates word frequency and probability of words occurring in spam or ham emails.
- Assigns labels to emails and computes classification probabilities.
- Allows testing on a separate dataset to evaluate classification accuracy.
- Supports threshold tuning for classification performance adjustments.

## File Structure
- `dataset_spam_train100.csv` : Spam training dataset (100 emails)
- `dataset_ham_train100.csv` : Ham (non-spam) training dataset (100 emails)
- `dataset_spam_test20.csv` : Spam testing dataset (20 emails)
- `dataset_ham_test20.csv` : Ham testing dataset (20 emails)
- `spamTrain.txt` : Processed spam training dataset
- `hamTrain.txt` : Processed ham training dataset
- `spam.txt` : Processed spam test dataset
- `ham.txt` : Processed ham test dataset

## How It Works
1. **Training Phase**
   - Calls `trainer()` function to read email datasets (`dataset_spam_train100.csv` and `dataset_ham_train100.csv`).
   - Processes text data by extracting meaningful words and removing non-alphabetic characters.
   - Saves cleaned data into `spamTrain.txt` and `hamTrain.txt`.
   - Computes word frequency and assigns probabilities using `calMail()`.

2. **Testing Phase**
   - Calls `testMail()` to preprocess test emails from `dataset_spam_test20.csv` and `dataset_ham_test20.csv`.
   - Compares words in test emails against trained word probabilities.
   - Computes spam probability score for each test email.
   - Classifies emails based on a user-defined threshold (default: 0.6).
   
3. **Evaluation**
   - Uses `calAccuracy()` to compare predictions with actual labels.
   - Prints results showing spam probability and classification outcome.
   - Calculates accuracy based on correct classifications.

## Compilation & Execution
### Compilation
Use a C compiler (e.g., GCC) to compile the program:
```sh
gcc spam_classifier.c -o spam_classifier
```

### Execution
Run the compiled program:
```sh
./spam_classifier
```
The program will automatically process training and test datasets and display classification results.

## Key Functions
### `trainer(char readFile[], char writeTxt[])`
- Reads raw email data from CSV files.
- Extracts meaningful words and writes them to a processed text file.

### `calMail(Mail *mail, int type)`
- Reads processed training data.
- Calculates word frequencies and assigns probabilities.

### `testMail(double *spamPercent, Mail *spam_mail, Mail *ham_mail, int type, int s_size, int h_size, char test[])`
- Reads test email dataset and computes spam probability for each email.

### `calAccuracy(double *test, float threshold, int type)`
- Evaluates classification results and computes accuracy.

## Adjustable Parameters
- **Threshold (`threshold`)**: Default value is `0.6`. Adjust for more aggressive or lenient spam classification.
  ```c
  float threshold = 0.6; // Modify this value to tune classification sensitivity
  ```

## Sample Output
```
 ____Spam____ 
(1) 0.89 : spam
(2) 0.75 : spam
(3) 0.42 : non Spam
...
======> Correct : 17

 ____non_Spam____ 
(1) 0.23 : non Spam
(2) 0.67 : Spam
(3) 0.12 : non Spam
...
======> Correct : 18

=====================
Threshold : 0.6 accuracy : 87.5%
=====================
```

## Notes
- Ensure that the required datasets are in the correct directory before execution.
- Modify `threshold` to fine-tune classification performance.
- The program assumes properly formatted CSV datasets without missing values.

## License
This project is for educational purposes only. Free to use and modify.

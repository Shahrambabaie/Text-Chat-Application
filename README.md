# 🧠 NN & CNN: Neural Networks and Convolutional Neural Networks

This project presents a complete deep-learning workflow using **Fully Connected Neural Networks (NNs)** and **Convolutional Neural Networks (CNNs)**.  
It walks through designing, training, optimizing, and evaluating neural architectures for both **tabular binary classification** and **image classification** tasks.

The project is organized into **five parts**, aligned with an increasingly advanced progression:

- **Part 1:** Build a Basic Neural Network (Binary Classification)  
- **Part 2:** Hyperparameter Optimization & Training Improvements  
- **Part 3:** Implement & Improve AlexNet for a Custom 3-Class Image Dataset  
- **Part 4:** Apply Improved CNN to SVHN with Data Augmentation  
- **Part 5:** Extended CNN Experiments & Additional Improvements  

---

## 📊 Dataset Overview

This project uses **three datasets**, each requiring distinct preprocessing pipelines.

---

### 🔹 Dataset 1 — Tabular Dataset (Parts 1 & 2)

Used for binary classification using a fully connected neural network.

**Structure:**
- 7 input features  
- 1 binary output label  
- Mix of numerical and categorical variables  

**Preprocessing steps:**
- One-hot encoding  
- Numerical feature standardization  
- Train/validation split  

---

### 🔹 Dataset 2 — Custom 3-Class Image Dataset (Part 3)

A dataset containing:

- 🐶 Dogs  
- 🚗 Cars  
- 🍔 Food  

**Properties:**
- 30,000 total images  
- 64×64 RGB format  
- Balanced across classes  

Used for training and improving an **AlexNet-based CNN**.

---

### 🔹 Dataset 3 — SVHN (Part 4)

The **Street View House Numbers (SVHN)** dataset contains:

- 600,000+ real-world digit images  
- 32×32 RGB format  
- High variation in lighting and backgrounds  

Used for evaluating model generalization under challenging conditions.

---

# 🧩 Part-by-Part Breakdown

---

## 📘 Part 1 — Building a Basic Neural Network (NN)

This section builds the foundational neural network for binary classification.

### ✔ Key steps:
- Data loading & preprocessing  
- Encoding categorical variables  
- Feature standardization  
- Define NN architecture (PyTorch or Keras)  
- Implement:
  - Forward pass  
  - Binary Cross-Entropy loss  
  - Backpropagation  
  - Optimizer (SGD/Adam)  
- Train/validation split  
- Compute accuracy and loss  

### 📈 Visualizations:
- Feature distributions  
- Correlation matrix  
- Training vs validation curves  
- Confusion matrix  

---

## 📘 Part 2 — Hyperparameter Optimization & Training Improvements

Improves NN performance through systematic hyperparameter tuning.

### ✔ Hyperparameters tested:
- Dropout  
- Optimizer choice  
- Activation functions  
- Weight initialization  

Each tested across **three configurations**.

### ✔ Optimization techniques:
- Early stopping  
- Learning rate scheduling  
- K-fold cross-validation  
- Regularization (Dropout, L2 penalty)  

### 📈 Outputs:
- Accuracy and loss curves  
- Performance comparison tables  
- Summary of best-performing configuration  

---

## 📘 Part 3 — Implementing & Improving AlexNet (Custom 3-Class CNN)

This part brings in deep convolutional networks with AlexNet.

### ✔ Steps:
- Load & visualize dataset  
- Preprocess images  
- Implement AlexNet architecture tailored to 64×64 images  
- Train, validate, and test the model  
- Apply improvements:
  - Dropout  
  - Batch normalization  
  - Learning rate schedules  
  - Architecture refinements  

### 📈 Results include:
- Accuracy vs. epoch curves  
- Confusion matrix  
- Sample predictions  
- Visualization of model architecture  

---

## 📘 Part 4 — CNN + Data Augmentation on SVHN

Tests the improved CNN on a realistic digit dataset.

### ✔ Techniques used:
- Augmentation:
  - Rotation  
  - Translation  
  - Horizontal flips  
  - Zoom  
  - Random cropping  
- Fine-tuning  
- Hyperparameter refinement  
- Evaluation on test split  

### 🎯 Target:
Achieve **94%+ accuracy** on SVHN.

---

## 📘 Part 5 — Extended CNN Experiments & Additional Improvements

This final section expands upon the CNN work by exploring additional architectures, training strategies, and analysis.

### ✔ Additional experiments include:
- Testing alternative CNN structures  
- Varying optimizers (Adam, SGD Momentum, RMSProp)  
- Experimenting with different learning rates  
- Using higher dropout or L2 regularization  
- Applying expanded augmentation pipelines  
- Comparing multiple training configurations  

### 📈 Outputs:
- Performance comparison tables  
- Training & validation curves for new configurations  
- Confusion matrices across extended experiments  
- Analysis of which improvements were most impactful  

---
```
## 📂 Project Structure

project/
├── part_1.ipynb # Part 1 & Part 2
├── part_2.ipynb # Part 3 & Part 4
├── part_3.ipynb # Part 5 (Extended CNN Experiments)
└── README.md # Project Documentation

```
---

## 🧰 Tools & Frameworks

- **Python 3.x**  
- **PyTorch or TensorFlow/Keras**  
- **NumPy**  
- **Pandas**  
- **Matplotlib**  
- **Seaborn**  
- **scikit-learn**

---

## 🎯 Key Learning Outcomes

- Understanding neural network architecture and training  
- Applying systematic hyperparameter tuning  
- Implementing AlexNet-style CNNs from scratch  
- Working with large-scale image datasets  
- Using data augmentation to improve generalization  
- Comparing models through structured experiments  

# Autocallable Certificate Pricing

This repository implements the pricing of an exotic derivative known as an **Autocallable Certificate** using **Monte Carlo simulation**. The project is written in **C++** and uses **QuantLib**, a powerful open-source library for quantitative finance.

In addition to pricing, the project estimates **model risk** by calculating the **replication error** based on the methodology proposed by **Derman and Kamal (2002)**.

---

## 📌 What is an Autocallable Certificate?

An **Autocallable Certificate** is a structured financial product that offers early redemption on predefined observation dates. On each observation date:

- If the underlying asset’s price is above a specified **autocall barrier**, the product is redeemed early, usually paying back the notional plus a fixed coupon.
- If not autocalled, the product continues to the next observation date or maturity.
- At maturity, additional features like **knock-in barriers** may affect the final payoff.

These products are widely used in structured investments and are characterized by non-linear and path-dependent payoffs.

---

## 🧮 Model Risk Estimation: Replication Error (Derman & Kamal)

To evaluate **model risk**, the project calculates the **replication error** using the approach described in:

> Derman, E., & Kamal, M. (2002). *The illusion of dynamic replication*. Risk Magazine, 15(8), 57-63.

### 📐 Formula (Simplified):

\[
\text{Model Risk} = \sqrt{ \mathbb{E} \left[ \left( V_{\text{model}} - V_{\text{replication}} \right)^2 \right] }
\]

Where:

- \( V_{\text{model}} \): Price from the Monte Carlo model  
- \( V_{\text{replication}} \): Value of a replicating portfolio or hedging strategy  
- The error reflects the potential deviation between the model price and actual hedge-based replication, acting as a proxy for model uncertainty

---

## ⚙️ Features

- Monte Carlo simulation engine for exotic payoff structures  
- Full customization of product parameters: barriers, schedule, coupons, maturity  
- Integration with QuantLib's stochastic processes and discounting models  
- Replication error computation for model risk assessment

---

## 🛠️ Technologies Used

- **C++17**  
- **QuantLib (C++)**  
- **CMake** for build configuration

---

## 🔧 Build Instructions

1. **Install QuantLib**:

```bash
git clone https://github.com/lballabio/QuantLib.git
cd QuantLib
./configure
make
sudo make install

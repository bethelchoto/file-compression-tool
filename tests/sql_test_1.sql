-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Mar 22, 2024 at 07:54 PM
-- Server version: 10.4.28-MariaDB
-- PHP Version: 8.2.4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `butcher_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `brands`
--

CREATE TABLE `brands` (
  `brand_id` int(11) NOT NULL,
  `brand_name` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

--
-- Dumping data for table `brands`
--

INSERT INTO `brands` (`brand_id`, `brand_name`) VALUES
(1, 'Chotel'),
(6, 'Chimupanz Cham-Cham'),
(7, 'Chots');

-- --------------------------------------------------------

--
-- Table structure for table `cart`
--

CREATE TABLE `cart` (
  `p_id` int(11) NOT NULL,
  `ip_add` varchar(50) NOT NULL,
  `c_id` int(11) DEFAULT NULL,
  `qty` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

-- --------------------------------------------------------

--
-- Table structure for table `categories`
--

CREATE TABLE `categories` (
  `cat_id` int(11) NOT NULL,
  `cat_name` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

--
-- Dumping data for table `categories`
--

INSERT INTO `categories` (`cat_id`, `cat_name`) VALUES
(1, 'Pork'),
(2, 'Beef'),
(4, 'Game Meat'),
(5, 'Mutton');

-- --------------------------------------------------------

--
-- Table structure for table `customer`
--

CREATE TABLE `customer` (
  `customer_id` int(11) NOT NULL,
  `customer_name` varchar(100) NOT NULL,
  `customer_email` varchar(50) NOT NULL,
  `customer_pass` varchar(150) NOT NULL,
  `customer_country` varchar(30) NOT NULL,
  `customer_city` varchar(30) NOT NULL,
  `customer_contact` varchar(15) NOT NULL,
  `customer_image` varchar(100) DEFAULT NULL,
  `user_role` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

--
-- Dumping data for table `customer`
--

INSERT INTO `customer` (`customer_id`, `customer_name`, `customer_email`, `customer_pass`, `customer_country`, `customer_city`, `customer_contact`, `customer_image`, `user_role`) VALUES
(9, 'choto', 'bethel.choto@ashesi.edu.gh', '$2y$10$4pJyTQjYFYevcT/iABXLzu6JbuX1iylVg/wEL2oydx45eASxs5OYy', 'Zimbabwe', 'Chinhoyi', '263786122009', '65f0a82e8027f.jpg', 2),
(10, 'bethel', 'chotobethel@gmail.com', '$2y$10$5c6VrZAjVJCI0Ou.UARfeebp57jzV6Kerw1U.ZIFyHtwHV8azgHfG', 'Zimbabwe', 'Chinhoyi', '2637861220', '65f0a9c5f2c33.jpg', 1);

-- --------------------------------------------------------

--
-- Table structure for table `orderdetails`
--

CREATE TABLE `orderdetails` (
  `order_id` int(11) NOT NULL,
  `product_id` int(11) NOT NULL,
  `qty` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

--
-- Dumping data for table `orderdetails`
--

INSERT INTO `orderdetails` (`order_id`, `product_id`, `qty`) VALUES
(20, 35, 3),
(20, 37, 4),
(21, 35, 1),
(23, 31, 1),
(24, 34, 1),
(24, 37, 1),
(24, 35, 1),
(25, 37, 1),
(25, 38, 1),
(25, 39, 1),
(25, 31, 3),
(25, 33, 1),
(25, 35, 1),
(26, 34, 1),
(26, 35, 1),
(26, 39, 1),
(26, 37, 1);

-- --------------------------------------------------------

--
-- Table structure for table `orders`
--

CREATE TABLE `orders` (
  `order_id` int(11) NOT NULL,
  `customer_id` int(11) NOT NULL,
  `invoice_no` int(11) NOT NULL,
  `order_date` date NOT NULL,
  `order_status` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

--
-- Dumping data for table `orders`
--

INSERT INTO `orders` (`order_id`, `customer_id`, `invoice_no`, `order_date`, `order_status`) VALUES
(20, 9, 765295345, '2024-03-20', 'pending'),
(21, 9, 207035125, '2024-03-20', 'pending'),
(23, 9, 1763010301, '2024-03-20', 'pending'),
(24, 9, 1600193099, '2024-03-22', 'pending'),
(25, 9, 132686888, '2024-03-22', 'pending'),
(26, 10, 2104755783, '2024-03-22', 'pending');

-- --------------------------------------------------------

--
-- Table structure for table `payment`
--

CREATE TABLE `payment` (
  `pay_id` int(11) NOT NULL,
  `amt` double NOT NULL,
  `customer_id` int(11) NOT NULL,
  `order_id` int(11) NOT NULL,
  `currency` text NOT NULL,
  `payment_date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

-- --------------------------------------------------------

--
-- Table structure for table `products`
--

CREATE TABLE `products` (
  `product_id` int(11) NOT NULL,
  `product_cat` int(11) NOT NULL,
  `product_brand` int(11) NOT NULL,
  `product_title` varchar(200) NOT NULL,
  `product_price` double NOT NULL,
  `product_desc` int(67) DEFAULT NULL,
  `product_image` varchar(100) DEFAULT NULL,
  `product_keywords` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;

--
-- Dumping data for table `products`
--

INSERT INTO `products` (`product_id`, `product_cat`, `product_brand`, `product_title`, `product_price`, `product_desc`, `product_image`, `product_keywords`) VALUES
(31, 1, 7, 'Discover', 200, 5, '65ea65248fb8a.png', 'sdfgrequired'),
(32, 1, 1, 'Discover the unmatched taste of our finest pork selections, meticulously raised and expertly crafted to deliver unparalleled flavor and tenderness.', 120, 20, '65eb18e34b447.jpg', 'good meat, good pork'),
(33, 2, 7, 'Indulge in the rich flavor and superior quality of our premium grass-fed beef. Sourced from practices, our beef is tender, succulent, and bursting with natural goodness.', 100, 30, '65eb2fc63b97d.jpg', 'specialrequiredrequired'),
(34, 2, 7, 'Indulge in the rich flavor and superior quality of our premium grass-fed beef. Sourced from practices, our beef is tender, succulent, and bursting with natural goodness.', 100, 20, '65ecada27a5ae.jpg', 'good meat, good pork'),
(35, 2, 7, 'Indulge in the rich flavor and superior quality of our premium grass-fed beef. Sourced from practices, our beef is tender, succulent, and bursting with natural goodness.', 100, 10, '65ee5544eabb2.jpg', 'good meat, good pork'),
(37, 4, 1, 'Discover the unmatched taste of our finest pork selections, meticulously raised and expertly crafted to deliver unparalleled flavor and tenderness.', 120, 20, '65ee5e1015a6d.jpg', 'Game Meat is Special'),
(38, 2, 1, 'Discover the unmatched taste of our finest pork selections, meticulously raised and expertly crafted to deliver unparalleled flavor and tenderness.', 100, 10, '65ee5eb5b31e6.jpg', 'good meat, good pork'),
(39, 2, 1, 'Discover the unmatched taste of our finest pork selections, meticulously raised and expertly crafted to deliver unparalleled flavor and tenderness.', 100, 20, '65ee5f8588465.jpg', 'Game Meat is Special'),
(40, 5, 1, 'Discover the unmatched taste of our finest pork selections, meticulously raised and expertly crafted to deliver unparalleled flavor and tenderness.', 200, 10, '65fdd016e9e2a.jpg', 'good meat, good mutton'),
(41, 1, 1, 'sdefrg', 200, 10, '65fdd0ca8c640.jpg', 'good meat, good pork');

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `username` varchar(50) NOT NULL,
  `email` varchar(100) NOT NULL,
  `password` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`id`, `username`, `email`, `password`) VALUES
(1, 'bethel', 'chotobethel@gmail.com', '1234566'),
(4, 'choto', 'choto@gmail.com', '12356'),
(5, 'chotel', 'bethel.choto@ashesi.edu.gh', '$2y$10$kLxhLHirDgLvqcL4Nyf8RetYCb1OrvhMnuv9kr81eYIA5O7Jt3G9.'),
(8, 'be', 'bethe.choto@ashesi.edu.gh', '$2y$10$TVzk/dEMxKe/J55A.SFu2Ow3y88hfEyaY3xfmpV3Aqd4z.NHLUdNq'),
(9, 'b', 'beth.choto@ashesi.edu.gh', '$2y$10$yvzmfMEzCNITuq/ZO3kWdeGUXmFxUeCBQK0e1CaZQ7WHcsx32Ccta'),
(11, 'bp', 'beth.choto@ashei.edu.gh', '$2y$10$MSIMQ64LlNdZbhKDtDCeWOjwbk6NEZdRPmJihL9RNJXAlw0RWgbz2'),
(12, 'bpm', 'beth.choto@kashei.edu.gh', '$2y$10$ycDTvYNneBa/oFiRdJJQpeiUrT2gz.1nB7MsfKIYKoMPz5Y6CcJRK'),
(13, 'Bethel Panashe Choto', 'bethl.choto@ashesi.edu.gh', '$2y$10$2O.iVFxCvxCfN1.ROE3SSOMgNdLRCCS7ryFOKGTfzWuFotcZ.ZpYK'),
(17, 'gfgfg', 'bethel.choto@ashesi.eduh', '2'),
(18, 'lue', 'choto@ashesi.eduh', '1'),
(26, 'chot', '', '1'),
(28, 'ch', 'chotel@gmail.com', '$2y$10$2JZivPU90OpKYqvAQOAKBeYFamH.03Ra8nJwx0i1fuGaB9bVQm0f6'),
(34, 'bb', 'bethel.choto@aesi.edu.gh', '$2y$10$yFWnLCRs/txU4aWFaPrnPO5ZeTSruXyad.Vh.Sq4ceYR3QkRZp8FO'),
(35, 'beee', 'bethelhoto@ashesi.edu.gh', '$2y$10$HBI1rJonH/lvHEpK0YrPSe2EeSgXPCoDZRw9e97N.ratUk9qwC75G');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `brands`
--
ALTER TABLE `brands`
  ADD PRIMARY KEY (`brand_id`);

--
-- Indexes for table `cart`
--
ALTER TABLE `cart`
  ADD KEY `p_id` (`p_id`);

--
-- Indexes for table `categories`
--
ALTER TABLE `categories`
  ADD PRIMARY KEY (`cat_id`);

--
-- Indexes for table `customer`
--
ALTER TABLE `customer`
  ADD PRIMARY KEY (`customer_id`),
  ADD UNIQUE KEY `customer_email` (`customer_email`);

--
-- Indexes for table `orderdetails`
--
ALTER TABLE `orderdetails`
  ADD KEY `order_id` (`order_id`),
  ADD KEY `product_id` (`product_id`);

--
-- Indexes for table `orders`
--
ALTER TABLE `orders`
  ADD PRIMARY KEY (`order_id`),
  ADD KEY `customer_id` (`customer_id`);

--
-- Indexes for table `payment`
--
ALTER TABLE `payment`
  ADD PRIMARY KEY (`pay_id`),
  ADD KEY `customer_id` (`customer_id`),
  ADD KEY `order_id` (`order_id`);

--
-- Indexes for table `products`
--
ALTER TABLE `products`
  ADD PRIMARY KEY (`product_id`),
  ADD KEY `product_cat` (`product_cat`),
  ADD KEY `product_brand` (`product_brand`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `username` (`username`),
  ADD UNIQUE KEY `email` (`email`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `brands`
--
ALTER TABLE `brands`
  MODIFY `brand_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=12;

--
-- AUTO_INCREMENT for table `categories`
--
ALTER TABLE `categories`
  MODIFY `cat_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=7;

--
-- AUTO_INCREMENT for table `customer`
--
ALTER TABLE `customer`
  MODIFY `customer_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT for table `orders`
--
ALTER TABLE `orders`
  MODIFY `order_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=27;

--
-- AUTO_INCREMENT for table `payment`
--
ALTER TABLE `payment`
  MODIFY `pay_id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `products`
--
ALTER TABLE `products`
  MODIFY `product_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=42;

--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=37;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `cart`
--
ALTER TABLE `cart`
  ADD CONSTRAINT `cart_ibfk_1` FOREIGN KEY (`p_id`) REFERENCES `products` (`product_id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `orderdetails`
--
ALTER TABLE `orderdetails`
  ADD CONSTRAINT `orderdetails_ibfk_1` FOREIGN KEY (`order_id`) REFERENCES `orders` (`order_id`),
  ADD CONSTRAINT `orderdetails_ibfk_2` FOREIGN KEY (`product_id`) REFERENCES `products` (`product_id`);

--
-- Constraints for table `orders`
--
ALTER TABLE `orders`
  ADD CONSTRAINT `orders_ibfk_1` FOREIGN KEY (`customer_id`) REFERENCES `customer` (`customer_id`);

--
-- Constraints for table `payment`
--
ALTER TABLE `payment`
  ADD CONSTRAINT `payment_ibfk_1` FOREIGN KEY (`customer_id`) REFERENCES `customer` (`customer_id`),
  ADD CONSTRAINT `payment_ibfk_2` FOREIGN KEY (`order_id`) REFERENCES `orders` (`order_id`);

--
-- Constraints for table `products`
--
ALTER TABLE `products`
  ADD CONSTRAINT `products_ibfk_1` FOREIGN KEY (`product_cat`) REFERENCES `categories` (`cat_id`),
  ADD CONSTRAINT `products_ibfk_2` FOREIGN KEY (`product_brand`) REFERENCES `brands` (`brand_id`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

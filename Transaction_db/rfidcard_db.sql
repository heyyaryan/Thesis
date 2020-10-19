-- phpMyAdmin SQL Dump
-- version 5.0.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Feb 01, 2020 at 07:35 AM
-- Server version: 10.4.11-MariaDB
-- PHP Version: 7.4.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `rfidcard_db`
--
CREATE DATABASE IF NOT EXISTS `rfidcard_db` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
USE `rfidcard_db`;

-- --------------------------------------------------------

--
-- Table structure for table `scratch_tbl`
--

CREATE TABLE `scratch_tbl` (
  `lastTransID` int(11) NOT NULL,
  `lastTransDate` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- Table structure for table `transaction_data`
--

CREATE TABLE `transaction_data` (
  `id_base` int(11) NOT NULL,
  `transactionID` int(11) NOT NULL,
  `transactionDateTime` datetime NOT NULL,
  `rfid_num` varchar(8) NOT NULL,
  `transactionAmount` decimal(6,2) NOT NULL DEFAULT 0.00,
  `transactionType` varchar(8) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `transaction_data`
--

INSERT INTO `transaction_data` (`id_base`, `transactionID`, `transactionDateTime`, `rfid_num`, `transactionAmount`, `transactionType`) VALUES
(3, 1, '2020-01-31 22:01:45', '12345678', '500.00', 'AM'),
(4, 2, '2020-01-31 22:45:16', '12345678', '500.00', 'AM'),
(5, 3, '2020-01-31 22:50:39', '', '500.00', 'AM'),
(6, 4, '2020-01-31 23:01:41', '', '700.00', 'AM'),
(7, 5, '2020-01-31 23:03:23', '12abcd34', '690.00', 'AM'),
(8, 6, '2020-01-31 23:03:58', '12abcd34', '690.00', 'AM'),
(9, 7, '2020-01-31 23:06:37', '11234559', '1200.00', 'AM'),
(10, 8, '2020-01-31 23:07:22', '11234559', '300.00', 'AM'),
(11, 9, '2020-01-31 23:22:31', '11234559', '750.00', 'AM'),
(12, 10, '2020-01-31 23:24:34', '12abcd34', '3500.00', 'AM'),
(13, 11, '2020-02-01 14:31:17', '12abcd34', '1200.00', 'AM'),
(14, 12, '2020-02-01 14:32:49', '12abcd34', '500.00', 'AM');

-- --------------------------------------------------------

--
-- Table structure for table `user_data`
--

CREATE TABLE `user_data` (
  `id_auto` int(11) NOT NULL,
  `rfid_num` varchar(8) NOT NULL,
  `userid` varchar(16) NOT NULL,
  `fname` varchar(64) NOT NULL,
  `lname` varchar(64) NOT NULL,
  `dept_ofc` varchar(8) NOT NULL,
  `balance` decimal(6,2) NOT NULL DEFAULT 0.00
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `user_data`
--

INSERT INTO `user_data` (`id_auto`, `rfid_num`, `userid`, `fname`, `lname`, `dept_ofc`, `balance`) VALUES
(153, '11234559', 'fd3335ad', 'john', 'doe', 'engg', '750.00'),
(154, '12abcd34', '69696969', 'johnny', 'sins', 'phub', '500.00'),
(155, '12345678', '8765asdf', 'John', 'Cena', 'ENGG', '0.00');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `transaction_data`
--
ALTER TABLE `transaction_data`
  ADD PRIMARY KEY (`id_base`);

--
-- Indexes for table `user_data`
--
ALTER TABLE `user_data`
  ADD PRIMARY KEY (`id_auto`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `transaction_data`
--
ALTER TABLE `transaction_data`
  MODIFY `id_base` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=15;

--
-- AUTO_INCREMENT for table `user_data`
--
ALTER TABLE `user_data`
  MODIFY `id_auto` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=156;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

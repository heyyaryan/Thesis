-- phpMyAdmin SQL Dump
-- version 4.9.0.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Dec 07, 2019 at 04:07 AM
-- Server version: 10.3.16-MariaDB
-- PHP Version: 7.3.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `transaction`
--

-- --------------------------------------------------------

--
-- Table structure for table `accounts_list`
--

CREATE TABLE `accounts_list` (
  `card_number` tinytext DEFAULT NULL,
  `first_name` varchar(50) DEFAULT NULL,
  `last_name` varchar(50) DEFAULT NULL,
  `department` tinytext DEFAULT NULL,
  `address` tinytext DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `accounts_list`
--

INSERT INTO `accounts_list` (`card_number`, `first_name`, `last_name`, `department`, `address`) VALUES
('0683394971\r\n', 'Ryan', 'David', 'BSCPE', 'Bacolod'),
('0683297275\r\n', 'Mira', 'Macario', 'BSECE', 'Negros'),
('hkjj', 'qweqw', 'qweq', 'adea', 'qweqw'),
('e4', '33', '33', '33', '33'),
('23', '123', '312', '123', '321');

-- --------------------------------------------------------

--
-- Table structure for table `transaction_history`
--

CREATE TABLE `transaction_history` (
  `transaction_number` tinytext DEFAULT NULL,
  `transaction_type` varchar(50) DEFAULT NULL,
  `amount` int(50) DEFAULT NULL,
  `card_number` tinytext DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

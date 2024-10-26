CREATE TABLE `devices_status` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `device_name` varchar(50) DEFAULT NULL,
  `device_status` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

INSERT INTO `devices_status` VALUES ('1', 'home_light', '1');
INSERT INTO `devices_status` VALUES ('2', 'shop_light', '0');

DROP PROCEDURE IF EXISTS `update_version_0`;

CREATE PROCEDURE update_version_0()
BEGIN
	CREATE TABLE `t_version` (
	  `id` varchar(64) NOT NULL,
	  `number` int(10) NOT NULL,
	  PRIMARY KEY (`id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	INSERT INTO `t_version` VALUES('VERSION', 0);
END;

CALL update_version_0();

DROP PROCEDURE IF EXISTS `update_version_0`;
DROP PROCEDURE IF EXISTS `check_version_0`;
DROP PROCEDURE IF EXISTS `update_version_1`;

CREATE PROCEDURE check_version_0(OUT ret INT)
BEGIN
	DECLARE num INT;
	SET ret = 0;
	SELECT `number` INTO num FROM `t_version` WHERE `id` = 'VERSION';
	IF num = 0 THEN
		SET ret = 1;
	END IF;
END;

CREATE PROCEDURE update_version_1()
BEGIN
	DECLARE ret INT;
	CALL check_version_0(ret);
	IF ret = 1 THEN
		
		CREATE TABLE `t_role` (
		  `id` bigint(20) NOT NULL,
		  `name` varchar(21) NOT NULL,
		  `data` blob,
		  `description` varchar(1024) DEFAULT NULL,
		  PRIMARY KEY (`id`)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8;

		CREATE TABLE `t_test` (
		  `id` bigint(20) NOT NULL,
		  `c` tinyint(3) NOT NULL,
		  `h` smallint(6) NOT NULL,
		  `d` int(10) NOT NULL,
		  `l` bigint(20) NOT NULL,
		  `uc` tinyint(3) NOT NULL,
		  `uh` smallint(6) NOT NULL,
		  `ud` int(10) NOT NULL,
		  `ul` bigint(20) NOT NULL,
		  `t` datetime NOT NULL,
		  `s` varchar(256) NOT NULL,
		  `b` blob NOT NULL,
		  `z` longblob NOT NULL,
		  PRIMARY KEY (`id`)
		) ENGINE=InnoDB DEFAULT CHARSET=utf8;

		UPDATE `t_version` SET `number` = 1 WHERE `id` = 'VERSION';
	END IF;
END;

CALL update_version_1();
DROP PROCEDURE IF EXISTS `check_version_0`;
DROP PROCEDURE IF EXISTS `update_version_1`;
drop table if exists my_user;
CREATE TABLE my_user (
  id int(11) NOT NULL,
  user_age int(11) NOT NULL,
  user_name varchar(255) DEFAULT NULL,
  PRIMARY KEY (id)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
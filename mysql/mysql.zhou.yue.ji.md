# mysql 获取本周本月本季开始结束时间

```sql
-- 本周始末，说这俩有问题，不知道哪里有问题 --
SELECT DATE_FORMAT( SUBDATE(CURDATE(),DATE_FORMAT(CURDATE(),'%w')-1), '%Y-%m-%d 00:00:00') AS '本周一';
SELECT DATE_FORMAT( SUBDATE(CURDATE(),DATE_FORMAT(CURDATE(),'%w')-7), '%Y-%m-%d 23:59:59') AS '本周末';

-- 本周始末 --
SELECT DATE_FORMAT( DATE_SUB(CURDATE(), INTERVAL WEEKDAY(CURDATE()) DAY), '%Y-%m-%d 00:00:00') AS '本周一';
SELECT DATE_FORMAT( DATE_ADD(SUBDATE(CURDATE(), WEEKDAY(CURDATE())), INTERVAL 6 DAY), '%Y-%m-%d 23:59:59') AS '本周末';

-- 本月始末 --
SELECT DATE_FORMAT( CURDATE(), '%Y-%m-01 00:00:00') AS '本月初';
SELECT DATE_FORMAT( LAST_DAY(CURDATE()), '%Y-%m-%d 23:59:59') AS '本月末';

-- 本季始末 --
select concat(DATE_FORMAT(LAST_DAY(MAKEDATE(EXTRACT(YEAR FROM CURDATE()),1) + interval QUARTER(CURDATE())*3-3 month),'%Y-%m-'),'01 00:00:00');
select concat(DATE_FORMAT(LAST_DAY(MAKEDATE(EXTRACT(YEAR FROM CURDATE()),1) + interval QUARTER(CURDATE())*3-1 month),'%Y-%m-%d'),' 23:59:59');
```

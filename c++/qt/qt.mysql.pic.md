# 在Qt程序中向mysql数据库保存和提取图片

## 建表插入数据

```sql
create table user( id int(4) not null auto_increment primary key, image longblob);

# 直接命令插入图片文件
# insert into user(image) values(LOAD_FILE('//root//horse.png'));
```

## 文件选择

```c++
QString imagePath = QFileDialog::getOpenFileName(this, tr("Open File"),"/root",tr("Images (*.png)"));
```

## 读取图片文件

```c++
QFile* file=new QFile(imagePath); //file为二进制数据文件名
file->open(QIODevice::ReadOnly);
QByteArray data file->readAll();
file->close();
```

## 写入图片文件

```c++
QByteArray bytes;
QBuffer buffer(&bytes);
buffer.open(QIODevice::WriteOnly);
ui->picLabel->pixmap()->save(&buffer, "PNG");
```

## 写入数据库

接下来代码是保存图片到数据库（以二进制形式保存）

```c++
QSqlQuery query;
QVariant var(data);
query.prepare("insert into hall_of_fame(head_image) values(?)");
query.addBindValue(var);
if(!query.exec())   qDebug()<<query.lastError().text().toLocal8Bit().data();
else    qDebug()<<"Insert success";
```

## 读取数据库

```c++
query.exec("select * from user");
if (query.next())
{
    QPixmap photo;
    photo.loadFromData(query.value(1).toBtyeArray(),"PNG");     //1为列的序号，这里0是id，1是image

    //图片无伸缩
    ui->picLabel->setPixmap(photo);
    //伸缩
    ui->picLabel->setScaledContents(true);
    //贴图时直接伸缩
    //ui->picLabel->setPixmap(QPixmap(photo).scaled(ui->picLabel->size()));
}
```

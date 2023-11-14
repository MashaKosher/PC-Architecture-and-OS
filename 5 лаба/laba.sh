#!/bin/bash

dir1=$1
dir2=$2

if [ -z "$dir1" ] || [ -z "$dir2" ]; then
  echo "Неверный ввод"
  echo "<$dir1> <$dir2>"
  exit 1
fi

count=0
for file1 in $dir1/*; do
  if [ -f "$file1" ]; then
    for file2 in $dir2/*; do
      if [ -f "$file2" ]; then
        if cmp "$file1" "$file2"; then
          echo "Файлы: $file1 и $file2 одинаковые"
        fi
      fi
    done
    count=$((count + 1))
  fi
done

if [ -d "$dir1" ] && [ -d "$dir2" ]; then
        if [ $count -gt 0 ]; then 
                echo "просмотренно файлов: $count"
            else
                echo "каталоги пустые"
        fi
    elif [ ! -d "$dir1" ] && [ ! -d "$dir2" ]; then 
        echo "нет таких каталогов"
    elif [ ! -d "$dir1" ] && [ -d "$dir2" ]; then 
        echo "есть каталог $dir2";
    else
        echo "нет каталога $dir2"  
fi
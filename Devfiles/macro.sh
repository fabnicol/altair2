#!/bin/bash
if (test -f "Exemple.html"); then 
 sed -i -r "s/(class=\"odd\"|class=\"header\")//g" Exemple.html 
 sed -i -r "s/class=\"even\"/class=\"fab\"/g" Exemple.html
fi
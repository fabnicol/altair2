#!/bin/bash
if (test -f "sierg.html"); then 
 sed -i -r "s/(class=\"odd\"|class=\"header\")//g" sierg.html 
 sed -i -r "s/class=\"even\"/class=\"fab\"/g" sierg.html
fi
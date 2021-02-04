#findlocation.sh

#Test case 1: no arguments passed:
if [ $# -eq 0 ]
  then
      echo "No arguments supplied"
      exit 1
fi

#Test case 2: argument not valid:
#Test if not 6 digits:
len_of_input=${#1}

if [ $len_of_input -ne 6 ]
then
    echo "Not 6 digit input"
    exit 1
fi
#Test if not digits:
if ! [[  $1 =~ ^[0-9]+$ ]]
then
    echo "Input not only digits 0-9"
    exit 1
fi

#Test cas 3: argument not found in nanpa
if grep -q $1 nanpa
then

    grep $1 nanpa | sed 's/......//' | sed 's/ *$//g' 
else
    echo "Not found in nanpa"
    exit 1
fi


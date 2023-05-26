import subprocess
import os

def test_wordcount_output():
    bin_dir = os.path.abspath('../build/bin')
    os.chdir(bin_dir)
    subprocess.run(['./wordcount'])

    output_dir = os.path.abspath('../../data/output')
    os.chdir(output_dir)

    # Ищем мэтчи
    expected_matches = ['''needs	1
night	1
no	7
nuts	1
of	3
once	1
one	1
oneself	1
or	1
our	1
palahniuk	1
pass	1
people	1
places	1
poor	1
read	1
safarli	1
sea	1
see	2
sheep	1
skies	1
slowly	1
smile	1
solitude	1
squirrels	1
stand	3
stare	3
stars	2
streams	1
tell	1
that	2
the	4
their	1
then	1
they	1
this	4
till	1
time	8
to	8
trained	1
turn	1
understand	1
wait	1
watch	1
way	1
we	3
what	1
when	1
where	1
woods	1
world	1
you	1
your	1''', '''a	3
about	1
all	1
and	7
animals	1
as	3
asleep	1
at	4
be	2
beauty’s	1
began	1
beneath	1
books	1
boughs	1
broad	1
can	2
care	2
chuck	1
club	1
completely	1
constantly	2
cows	1
dance	1
daylight	1
deeds	1
elchin	1
emotions	1
ending	1
enrich	1
eyes	1
fall	1
fault	1
favorite	1
feet	1
fell	1
fight	1
full	3
glance	1
good	1
grass	1
green	1
have	2
he	1
her	3
hide	1
how	1
i	2
if	2
in	5
is	3
its	1
john	1
life	3
like	2
long	1
love	3
maintained	1
me	1
moment	1
mouth	1
muscle	1
must	1'''] 
    #"As\t1\nGood\t1\nSafarli\t1\nWhere\t1\nat\t4\ncare\t2\nending\t1\ngrass\t1\nis\t3\nmoment\t1\nof\t3\npoor\t1\nsolitude\t1\nthen\t1\nunderstand\t1\nyou\t1",
    #"Beauty’s\t1\nGreen\t1\nStars\t1\na\t2\nbe\t2\ncompletely\t1\neyes\t1\nhave\t2\nits\t1\nmouth\t1\nonce\t1\nread\t1\nsquirrels\t1\nthey\t1\nwait\t1\nyour\t1",
    #"Chuck\t1\nI\t2\nStreams\t1\nabout\t1\nbegan\t1\nconstantly\t2\nfall\t1\nhe\t1\nlife\t3\nmuscle\t1\none\t1\nsea\t1\nstand\t3\nthis\t3\nwatch\t1",
    #"Elchin\t1\nLove\t2\nThe\t1\nand\t5\nbooks\t1\ndance\t1\nfeet\t1\nhide\t1\nlong\t1\nneeds\t1\nor\t1\nsheep\t1\nstars\t1\ntime\t8\nwe\t1",
    #"Club\t1\nJohn\t1\nTell\t1\nall\t1\nbeneath\t1\ncows\t1\nfavorite\t1\nher\t3\nlike\t2\nmust\t1\noneself\t1\nsee\t2\nstare\t3\ntill\t1\nway\t1",
    #"Enrich\t1\nNo\t5\nThis\t1\nanimals\t1\nboughs\t1\ndaylight\t1\nfell\t1\nhow\t1\nlove\t1\nnight\t1\npass\t1\nskies\t1\nthat\t2\nto\t8\nwhen\t1"] 


    match_found = [False] * len(expected_matches)

    # Ищем совпадения в каждом файле output директории
    for filename in os.listdir():
        with open(filename, 'r') as file:
            content = file.read().strip()
            for i, expected_match in enumerate(expected_matches):
                if expected_match in content:
                    match_found[i] = True


    all_matches_found = all(match_found)

    if all_matches_found:
        print("Все совпадения найдены!")
    else:
        print("Не всё найдено. Где-то ошибочка")

    print("Integration test completed!!!")

test_wordcount_output()

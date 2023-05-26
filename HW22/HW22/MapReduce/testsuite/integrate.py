import subprocess
import os

def test_wordcount_output():
    bin_dir = os.path.abspath('../build/bin')
    os.chdir(bin_dir)
    subprocess.run(['./wordcount'])

    output_dir = os.path.abspath('../../data/output')
    os.chdir(output_dir)

    # Ищем мэтчи
    expected_matches = ["And\t2\nFight\t1\nPalahniuk\t1\nWhat\t1\nasleep\t1\ncan\t2\nemotions\t1\nglance\t1\nin\t5\nme\t1\nnuts\t1\nplaces\t1\nsmile\t1\ntheir\t1\nturn\t1\nworld\t1",
    "A\t1\nFault\t1\nOur\t1\nWe\t2\nas\t2\nbroad\t1\ndeeds\t1\nfull\t3\nif\t2\nmaintained\t1\nno\t2\npeople\t1\nslowly\t1\nthe\t3\ntrained\t1\nwoods\t1", 
    "As\t1\nGood\t1\nSafarli\t1\nWhere\t1\nat\t4\ncare\t2\nending\t1\ngrass\t1\nis\t3\nmoment\t1\nof\t3\npoor\t1\nsolitude\t1\nthen\t1\nunderstand\t1\nyou\t1",
    "Beauty’s\t1\nGreen\t1\nStars\t1\na\t2\nbe\t2\ncompletely\t1\neyes\t1\nhave\t2\nits\t1\nmouth\t1\nonce\t1\nread\t1\nsquirrels\t1\nthey\t1\nwait\t1\nyour\t1",
    "Chuck\t1\nI\t2\nStreams\t1\nabout\t1\nbegan\t1\nconstantly\t2\nfall\t1\nhe\t1\nlife\t3\nmuscle\t1\none\t1\nsea\t1\nstand\t3\nthis\t3\nwatch\t1",
    "Elchin\t1\nLove\t2\nThe\t1\nand\t5\nbooks\t1\ndance\t1\nfeet\t1\nhide\t1\nlong\t1\nneeds\t1\nor\t1\nsheep\t1\nstars\t1\ntime\t8\nwe\t1",
    "Club\t1\nJohn\t1\nTell\t1\nall\t1\nbeneath\t1\ncows\t1\nfavorite\t1\nher\t3\nlike\t2\nmust\t1\noneself\t1\nsee\t2\nstare\t3\ntill\t1\nway\t1",
    "Enrich\t1\nNo\t5\nThis\t1\nanimals\t1\nboughs\t1\ndaylight\t1\nfell\t1\nhow\t1\nlove\t1\nnight\t1\npass\t1\nskies\t1\nthat\t2\nto\t8\nwhen\t1"] 


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

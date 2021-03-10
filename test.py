import os
import subprocess

def run_tests(test_dir, result):
    if os.path.isdir(test_dir):
        for root, subdirectories, files in os.walk(test_dir):
            for file in files:
                if file.startswith("input"):
                    print("Testing...", file)

                    # get pieces
                    file_parts = file.split('-')
                    test_num = file_parts[0][5:]
                    test_width = file_parts[1].split('.')[0]
                    input_path = '/'.join([test_dir, file])

                    # run word-wrapper
                    ww_command = "./ww {} {}".format(test_width, input_path)
                    try:
                        ww_output = subprocess.check_output(ww_command.split())
                    except subprocess.CalledProcessError as exc:
                        ww_output = exc.output

                    # put output into temp output file
                    with open('ww_output.txt', 'w+') as ww_output_file:
                        formatted_ww_output = str(ww_output).replace('\\n', '\n').replace('\\t', '\t')
                        formatted_ww_output = formatted_ww_output.split('\'')[1]
                        ww_output_file.write(formatted_ww_output)

                    # compare output
                    output_file = ''.join(['output', test_num, '.txt'])
                    output_path = '/'.join([test_dir, output_file])

                    rc = 0
                    cmp_command = "cmp {} {}".format('ww_output.txt', output_path)
                    try:
                        subprocess.check_output(cmp_command.split())
                    except subprocess.CalledProcessError as exc:
                        rc = exc.returncode

                    if rc == 0:
                        result[file] = "passed"
                    else:
                        result[file] = "failed"
    
    print()

def print_result(result):
    for key, value in result.items():
        print(key, ':', value)

if __name__ == "__main__":
    test_dir = "testfiles"
    result = {}

    run_tests(test_dir, result)
    print_result(result)
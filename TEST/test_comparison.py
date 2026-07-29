# references: https://www.w3schools.com/python/python_file_handling.asp
# https://www.geeksforgeeks.org/python/how-to-read-specific-lines-from-a-file-in-python/
# 
#  get the file of the test results

def linear(fail_list, target):

    for i in fail_list:
        if target == i:
            return True
    return False

results = open(r"C:\\Users\\Sebastian\\OneDrive\\Documents\\GitHub\\M-U-E-S\\build\\nestest_out.txt", "rt",encoding='utf-16')
results_content = results.readlines()
final = open(r"C:\\Users\\Sebastian\\Downloads\\nestest.log", "rt")
final_content = final.readlines()
example = results_content[0]

test_pass = True
fail_index = 0
fail_list = []
# loop through the the entire results file
for i in range(0,1000):
    # set the line to be extracted
    result_line = results_content[i]
    final_line = final_content[i]
    # extract the contents of each file
    res_opcode = result_line[0:4].upper()
    res_pc = result_line[7:12].upper()
    res_acc = result_line[16:19].upper()
    res_x = result_line[21:24].upper()
    res_y = result_line[26:29].upper()
    res_status = result_line[41:44].upper()
    res_stptr = result_line[54:55].upper()
    
    fin_opcode = final_line[16:19].upper()
    fin_pc = final_line[0:4].upper()
    fin_acc = final_line[50:53].upper()
    fin_x = final_line[55:58].upper()
    fin_y = final_line[60:63].upper()
    fin_status = final_line[65:68].upper()
    fin_stptr = final_line[71:74].upper()

    # check if they match
    if (res_opcode != fin_opcode) and (res_pc != fin_pc) and (res_acc != fin_acc) and (res_x != fin_x) and (res_y != fin_y) and (res_status != fin_status) and (res_stptr != fin_stptr):
        print(results_content[i].upper())
        print(final_content[i].upper())
        if(linear(fail_list, res_opcode)) == False:
            fail_list.append(res_opcode)
        test_pass = False
        fail_index += 1

if test_pass == True:
    print("Pass")
else:
    print("Fail")
    for i in fail_list:
        print(i)
print("closing files")
results.close()
final.close()
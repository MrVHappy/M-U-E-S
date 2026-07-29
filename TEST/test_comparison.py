# references: https://www.w3schools.com/python/python_file_handling.asp
# https://www.geeksforgeeks.org/python/how-to-read-specific-lines-from-a-file-in-python/
# 
#  get the file of the test results
results = open(r"C:\\Users\\Sebastian\\OneDrive\\Documents\\GitHub\\M-U-E-S\\build\\nestest_out.txt", "rt",encoding='utf-16')
results_content = results.readlines()
final = open(r"C:\\Users\\Sebastian\\Downloads\\nestest.log", "rt")
final_content = final.readlines()
example = results_content[0]

test_pass = True
fail_index = 0
# loop through the the entire results file
for i in range(0,1000):
    # set the line to be extracted
    result_line = results_content[i]
    final_line = final_content[i]
    # extract the contents of each file
    res_opcode = result_line[15:18].upper()
    res_pc = result_line[22:26].upper()
    res_acc = result_line[31:33].upper()
    res_x = result_line[36:38].upper()
    res_y = result_line[41:43].upper()
    res_status = result_line[56:58].upper()
    res_stptr = result_line[69:71].upper()
    
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
        test_pass = False
        fail_index += 1

if test_pass == True:
    print("Pass")
else:
    print("Fail")
    print(fail_index)

results.close()
final.close()
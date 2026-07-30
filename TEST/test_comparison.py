# references: https://www.w3schools.com/python/python_file_handling.asp
# https://www.geeksforgeeks.org/python/how-to-read-specific-lines-from-a-file-in-python/
# 
#  get the file of the test results

def linear(fail_list, target):

    for i in fail_list:
        if target == i:
            return True
    return False
first_error = 0
results = open(r"C:\\Users\\Sebastian\\OneDrive\\Documents\\GitHub\\M-U-E-S\\build\\nestest_out.txt", "rt",encoding='utf-16')
results_content = results.readlines()
final = open(r"C:\\Users\\Sebastian\\Downloads\\nestest.log", "rt")
final_content = final.readlines()
example = results_content[0]
first_mismatch = ""
prev_mismatch = ""
test_pass = True
fail_index = 0
# stores instructions that failed
fail_list = []
# stored instructions that lead to a failed instruction
prev_list = []
# loop through the the entire results file
for i in range(0,len(results_content)):
    # set the line to be extracted
    result_line = results_content[i]
    final_line = final_content[i]
    # extract the contents of each file
    res_opcode = result_line[0:4].strip().upper()
    res_pc = result_line[7:12].strip().upper()
    res_acc = result_line[16:19].strip().upper()
    res_x = result_line[21:24].strip().upper()
    res_y = result_line[26:29].strip().upper()
    res_status = result_line[41:44].strip().upper()
    res_stptr = result_line[54:56].strip().upper()

    
    fin_opcode = final_line[16:19].strip().upper()
    fin_pc = final_line[0:4].strip().upper()
    fin_acc = final_line[50:53].strip().upper()
    fin_x = final_line[55:58].strip().upper()
    fin_y = final_line[60:63].strip().upper()
    fin_status = final_line[65:68].strip().upper()
    fin_stptr = final_line[71:74].strip().upper()

    # check if they match
    if (res_opcode != fin_opcode) or (res_pc != fin_pc) or (res_acc != fin_acc) or (res_x != fin_x) or (res_y != fin_y) or (res_status != fin_status) or (res_stptr != fin_stptr):
        # display disparrety
        if first_mismatch == "":
            first_error = i
            first_mismatch = result_line
        print("disparrety")
        print("Result:\n" \
        "opcode: " + res_opcode + " PC: " + res_pc +
        " ACC: " + res_acc + " X: " + res_x + 
        " Y: " + res_y + " P: " + res_status +
        " STK: " + res_stptr)
        print("Final:\n" \
                "opcode: " + fin_opcode + " PC: " + fin_pc +
                " ACC: " + fin_acc + " X: " + fin_x + 
                " Y: " + fin_y + " P: " + fin_status +
                " STK: " + fin_stptr)
        
        # check if the instruction has been added to the fail list
        if(linear(fail_list, res_opcode)) == False:
            fail_list.append(res_opcode)
        # update the res opcode to check the prior instruction
        result_line = results_content[i-1]
        final_line = final_content[i -1]

        print("prior instruction")
        if prev_mismatch == "":
            prev_mismatch = result_line

        res_opcode = result_line[0:4].strip().upper()
        res_pc = result_line[7:12].strip().upper()
        res_acc = result_line[16:19].strip().upper()
        res_x = result_line[21:24].strip().upper()
        res_y = result_line[26:29].strip().upper()
        res_status = result_line[41:44].strip().upper()
        res_stptr = result_line[54:56].strip().upper()
        
        fin_opcode = final_line[16:19].strip().upper()
        fin_pc = final_line[0:4].strip().upper()
        fin_acc = final_line[50:53].strip().upper()
        fin_x = final_line[55:58].strip().upper()
        fin_y = final_line[60:63].strip().upper()
        fin_status = final_line[65:68].strip().upper()
        fin_stptr = final_line[71:74].strip().upper()
        # used for diagnostics
        print("Result:\n" \
                "opcode: " + res_opcode + " PC: " + res_pc +
                " ACC: " + res_acc + " X: " + res_x + 
                " Y: " + res_y + " P: " + res_status +
                " STK: " + res_stptr)
        print("Final:\n" \
                "opcode: " + fin_opcode + " PC: " + fin_pc +
                " ACC: " + fin_acc + " X: " + fin_x + 
                " Y: " + fin_y + " P: " + fin_status +
                " STK: " + fin_stptr)
        print("next")
        # check if the instruction has been added to the prev list
        if(linear(prev_list, res_opcode)) == False:
            prev_list.append(res_opcode)
        test_pass = False
        fail_index += 1

if test_pass == True:
    print("Pass")
else:
    print("Fail")
    print("Failed instructions")
    for i in fail_list:
        print(i)
    print("instructions prior to failure")
    for i in prev_list:
        print(i)

print(first_mismatch)
print(prev_mismatch)
print("first error:",first_error)
print(fail_index, "/", len(results_content) )
print("closing files")
results.close()
final.close()
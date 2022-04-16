# 这是一个示例 Python 脚本。

# 按 Shift+F10 执行或将其替换为您的代码。
# 按 双击 Shift 在所有地方搜索类、文件、工具窗口、操作和设置。


import linecache
import os
import json
import codecs
def count_char(str,ch):
    counter = 0
    for i in str:
        if i==ch:
            counter+=1
    return counter
def get_all_txt(path):
    L=[]
    for file in os.listdir(path):
        file_path = os.path.join(path,file)
        if(os.path.splitext(file_path)[1]=='.txt'):
#            L.append(os.path.splitext(file_path)[0])
            L.append(file.split('.')[0])
    return L
def get_all_json(path):
    L=[]
    for file in os.listdir(path):
        file_path = os.path.join(path,file)
        if(os.path.splitext(file_path)[1]=='.json'):
#            L.append(os.path.splitext(file_path)[0])
            L.append(file.split('.')[0])
    return L

old_txt_path = 'C:/Users/admin/Desktop/translation_original/'
old_json_path = 'C:/Users/admin/Desktop/translation_oldjson/'
new_path = 'C:/Users/admin/Desktop/translation_newjson/'
TR_TXT=[]
TR_JSON=[]
TR_TXT = get_all_txt(old_txt_path)
TR_JSON = get_all_json(old_json_path)
num = 0
c_num = 0
key_num = 0
for txt_name in TR_TXT:
    num += 1
    f = open(old_txt_path + txt_name + '.txt', 'r', encoding='utf-8-sig', errors='ignore')
    h = open(new_path+ txt_name + '.json','w',encoding='utf-8',errors='ignore')
    key_proceeding = 0
    key = 0
    json_data = []
    for line in f:
        line_strip_tmp = line.strip()
        if (line[0] == '\t' and line[1] == '}'):
            key_proceeding = 0
        if (count_char(line, '\"') == 2 and line[0] != '\"'):
            json_ele = {}
            key_proceeding = 1
            key_original = line.split('\"')[1]
            json_ele['key'] = '\"'+str(key)+'\"'
            json_ele['original'] = key_original
            json_ele['translation'] = ''
            json_data.append(json_ele)
            key += 1
    for json_name in TR_JSON:
        if(json_name == txt_name):
            g = open(old_json_path + json_name + '.json', 'r', encoding='utf-8-sig', errors='ignore')
            c_num+=1
            key_proceeding = 0
            old_json_data =json.load(g)
            trans_log_list = []
            old_json_keynum = 0
            for ele in old_json_data:
                for jele in json_data:
                    if(jele['original']==ele['original']):
                        jele['translation']=ele['translation']
            g.close()
            break
    print(json_data)
    h.write(json.dumps(json_data, indent=4,ensure_ascii=False))
    f.close()
    h.close()






# 访问 https://www.jetbrains.com/help/pycharm/ 获取 PyCharm 帮助

# 这是一个示例 Python 脚本。

# 按 Shift+F10 执行或将其替换为您的代码。
# 按 双击 Shift 在所有地方搜索类、文件、工具窗口、操作和设置。


# 按间距中的绿色按钮以运行脚本。
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
old_path = 'C:/Users/admin/Desktop/TRANSLATION/'
new_path = 'C:/Users/admin/Desktop/translation_convert_out/'
TR_JSON=[]
TR_JSON=get_all_json(old_path)
TR_TXT=[]
c_num = 0;
num = 0;
TR_TXT=get_all_txt(old_path)
for json_name in TR_JSON:
    num+=1;
    for txt_name in TR_TXT:
        g = open (old_path+json_name+'.json','r',encoding='utf-8-sig',errors='ignore')
        if(json_name == txt_name):
            json_data = json.load(g)
            trans_log_list = []
            h = codecs.open(new_path + txt_name + '.txt', 'w','utf8',errors='ignore')
            f = open(old_path+txt_name+'.txt','r',encoding='utf-8-sig',errors='ignore')
            for ele in json_data:
                t_log = {}
                t_log['original'] = ele['original']
                t_log['translation'] = ele['translation']
                trans_log_list.append(t_log);
            key = -1
            key_proceeding = 0
            for line in f:
                if(key_proceeding == 1):
                    line_tmp = line.strip()
                    if(line[0]!='}' and line_tmp== '}' ):
                        key_proceeding = 0
                        h.writelines('\n'+line)
                        continue
                    if(line_tmp.find('\"default\"')==-1 and line_tmp.find('\"ChatHud\"')==-1):
                        h.writelines(line)
                        continue
                    if(line_tmp.find('\"default\"')!=-1):
                        if(key_transfound!=0):
                            if(sayout_translate.find('►')!=-1):
                                h.writelines('\t\t\"default\"\t\t\"'+sayout_translate+'\"\n')
                            else:
                                h.writelines('\t\t\"default\"\t\t\"{red}[地图消息] {yellow}► {green} '+sayout_translate+' {yellow}◄"\n')
                        else:
                            h.writelines(line)
                        continue
                    if(line_tmp.find('\"ChatHud\"')!=-1):
                        if(key_transfound!=0):
                            if(sayout_translate.find('{red}[地图消息] {yellow}► {green} ')!=-1):
                                chathud_sayout_temp = sayout_translate.split('{red}[地图消息] {yellow}► {green} ')[1]
                                chathud_sayout_temp = chathud_sayout_temp.split(' {yellow}◄')[0]
                            else:
                                chathud_sayout_tempt= sayout_translate
                            h.writelines('\t\t\"ChatHud\"\t\t"► '+sayout_translate+' ◄\"\n')
                            continue

                if(count_char(line,'\"')==2):
                    key+=1;
                    if(line[0]=='\t'):
                        sayout_original=line.split("\"",1)[1]
                        sayout_original = sayout_original.rsplit("\"",1)[0]
                        key_proceeding = 1
                        key_transfound = 0
                        for ele in trans_log_list:
                            if(ele['original']==sayout_original):
                                sayout_translate = ele['translation']
                                key_transfound = 1;
                                break
                h.writelines(line)
            c_num+= 1
            if(key==0):
                h.writelines('}')
            f.close()
            h.close()

        g.close()
print(num);
print(c_num);


# 访问 https://www.jetbrains.com/help/pycharm/ 获取 PyCharm 帮助

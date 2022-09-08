# 这是一个示例 Python 脚本。

# 按 Shift+F10 执行或将其替换为您的代码。
# 按 双击 Shift 在所有地方搜索类、文件、工具窗口、操作和设置。

import datetime
import time
def print_hi(name):
    # 在下面的代码行中使用断点来调试脚本。
    print(f'Hi, {name}')  # 按 Ctrl+F8 切换断点。

def get_timestamp_spec(clock,days=0):
#   clock=钟点 days=与本日的日数差
    nowTime = datetime.datetime.now()+datetime.timedelta(days=days)
    print(nowTime)
    specified_time = nowTime.strftime("%Y-%m-%d") + " {}:00:00".format(clock)
    print(specified_time)
    time_Array = time.strptime(specified_time,"%Y-%m-%d %H:%M:%S")
    return time.mktime(time_Array)

# 按间距中的绿色按钮以运行脚本。
if __name__ == '__main__':
    print_hi('PyCharm')
print(str(get_timestamp_spec(4,120)))
# 访问 https://www.jetbrains.com/help/pycharm/ 获取 PyCharm 帮助

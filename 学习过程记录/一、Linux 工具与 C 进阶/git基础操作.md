# 初始化仓库
        git init 
        # 本地仓库提交
        git commit -m "first commit"
        # 添加要提交的文件
        git add ./
        git add -A  #添加所有文件
        # 确认暂存区内容
        git status
        # 创建提交
        git commit -m "Add AXU5EV-P documentation"
        # 推送到 GitHub 的 main 分支
        git push -u origin main
  
#管理分支 git branch
 #常用命令：
        # 查看分支
        git branch
        # 创建分支
        git branch 分支名
        # 删除本地分支
        git branch -d 分支名
        # 删除远程分支：
        git push origin --delete 分支名
        # 切换到已有本地分支：
        git switch 分支名
        # 从远程分支创建本地分支并切换
        git switch -c develop --track origin/develop
    #回退代码
    1. 丢弃尚未暂存的文件修改：
        git restore 文件名
        # 丢弃所有未暂存修改
        git restore .
    2. 取消 git add，但保留文件内容：
        git restore --staged 文件名
         # 取消全部暂存
        git restore --staged .
    3. 回退最近一次提交，但保留修改内容，方便重新提交：
        git reset --soft HEAD~1
    4. 回退最近一次提交，并取消暂存，但保留文件修改：
        git reset HEAD~1
    5. 彻底回退最近一次提交，连文件修改也丢弃：
        git reset --hard HEAD~1
    如果提交已经推送到 GitHub，推荐用“反向提交”而不是改写历史：
        git revert HEAD
        git push
    它会新建一个提交，用来撤销上一次提交，适合公共分支（如 main）。
    如果确实需要让远程分支强制回到旧提交：
        git reset --hard 提交ID
        git push --force-with-lease origin main


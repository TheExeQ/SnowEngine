@echo off
set file=%~n1
start "" fbx2ozz.exe --file="%file%.fbx" --config="{\"skeleton\":{\"filename\":\"%file%.skeleton\"},\"animations\":[{\"filename\":\"*.animation\"}]}"
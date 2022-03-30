
# 安装使用

## 安装教程

1. 将插件zip包放入paracraft安装目录下的Mod目录中
2. 在paracraft启动设置中启用当前插件(MC)

## Paracraft 与 Minecraft 方块对应规则

Minecraft 方块:  https://minecraftitemids.com/

Zip 中的Mod/MC/blocks.xml文件可以配置两者的对应关系:

```xml
<blocks>
  <!--mc_name: minecraft 方块名称 pc_id: paracraft 方块Id pc_data: paracraft 方块数据 其它字段用于MineCraft较旧版本转换(旧版本也是使用的方块Id), 可选默认为0. mc_id: minecraft 方块ID mc_data: minecraft 方块数据-->
 <block mc_name="minecraft:air" mc_id="0" mc_data="0" mc_state="0" pc_id="0" pc_data="0" pc_side="0"></block>
 <block mc_name="minecraft:copper_ore" mc_id="0" mc_data="0" mc_state="0" pc_id="18" pc_data="0" pc_side="0"></block>
</blocks>
```

## 使用方式

使用插件进入世界后, 插件提供了 mc 命令, 支持如下几种功能:

1. `/mc load_world world_dir` 加载 minecraft 世界
2. `/mc load_schematics schematic_filepath`  加载 minecraft 的 schematics 区域地图文件
3. `/mc load_block_config blocks.xml` 加载 Paracraft 与 Minecraft 方块对应规则文件, 可重写对应规则
4. `/mc debug` 打印使用过程中遇到没有配置两者方块对应规则的调试信息

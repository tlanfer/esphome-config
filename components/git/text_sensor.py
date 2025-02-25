import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import (
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_NEW_BOX,
)

CODEOWNERS = ["@tlanfer"]

git_ns = cg.esphome_ns.namespace("git_commit_sha")
GitTextSensor = git_ns.class_(
    "GitTextSensor", text_sensor.TextSensor, cg.Component
)

CONFIG_SCHEMA = (
    text_sensor.text_sensor_schema(
        icon=ICON_NEW_BOX,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    )
    .extend(
        {
            cv.GenerateID(): cv.declare_id(GitTextSensor),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = await text_sensor.new_text_sensor(config)
    await cg.register_component(var, config)

    try:
        with open(".git/HEAD", "r") as f:
            head = f.read().strip()

        with open(".git/" + head.split(": ")[1], "r") as f:
            git_commit = f.read().strip()

        version_tag = git_commit[:7]
    except OSError:
        version_tag = "unknown"

    cg.add(var.set_git_commit_sha(version_tag))

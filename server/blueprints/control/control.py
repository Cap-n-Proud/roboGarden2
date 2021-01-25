from flask import Blueprint
from flask import current_app as app
from blueprints.api import getPlantsDB, getPrograms, getCurrentProgr
from flask import render_template
import config

# Blueprint Configuration
control_bp = Blueprint(
    "control_bp", __name__, template_folder="templates", static_folder="static"
)


@control_bp.route("/control", methods=["POST", "GET"])
def status():
    """Control page."""
    programs = getPrograms()
    currentProgr = getCurrentProgr()
    return render_template(
        "indexControl.jinja2",
        title="Control",
        subtitle="Use this to control lights, pump and other features.",
        template="control-template",
        programs=programs,
        currentProgram=getCurrentProgr,
        INFOTAG=config.Config.INFOTAG,
        TELEMETRYTAG=config.Config.TELEMETRYTAG,
    )

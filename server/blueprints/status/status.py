from blueprints.api import getPlantsDB, getStatus, getCurrentProgr
from flask import Blueprint
from flask import current_app as app
from flask import render_template
import config
from blueprints.init import timeStarted

# status = getStatus()
# currentProgram = getCurrentProgram()
# programs = getPrograms()

# Blueprint Configuration
status_bp = Blueprint(
    "status_bp", __name__, template_folder="templates", static_folder="static"
)


@status_bp.route("/status", methods=["POST", "GET"])
def status():
    """Status page."""
    plantsDB = getPlantsDB(app)
    status = getStatus()
    # print(config.Config.ASSETS_FOLDER)
    return render_template(
        "index.jinja2",
        title="Status",
        subtitle="Control the status of roboGarden and the status of the pods.",
        template="status-template",
        plantsDB=plantsDB,
        status=status,
        INFOTAG=config.Config.INFOTAG,
        TELEMETRYTAG=config.Config.TELEMETRYTAG,
        timeStarted=timeStarted,
        currentProgram=getCurrentProgr(),
    )

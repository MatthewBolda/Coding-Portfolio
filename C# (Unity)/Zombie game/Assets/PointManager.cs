using UnityEngine;
using UnityEngine.UI;

public class PointManager : MonoBehaviour
{
    public Player player;
    public Text pointText;

    // Update is called once per frame
    void Update()
    {
        pointText.text = player.currentPoints.ToString("0");
    }
}

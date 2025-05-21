using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    [Header("UI References")]
    public TMP_Text p1Time;
    public TMP_Text p2Time;
    public TMP_Text p1Score;
    public TMP_Text p2Score;
    public Image p1Panel;
    public Image p2Panel;
    public TMP_Text difficultyText;

    private int sessionP1Score = 0;
    private int sessionP2Score = 0;

    void OnEnable() => UnitySerialPort.SerialDataParseEvent += OnSerialData;
    void OnDisable() => UnitySerialPort.SerialDataParseEvent -= OnSerialData;

    void Start()
    {
        sessionP1Score = sessionP2Score = 0;
        UpdateSessionScoresUI();
    }

    void OnSerialData(string[] data, string raw)
    {
        if (raw == "START")
        {
            ResetRoundUI();
            return;
        }

        if (raw.StartsWith("DIFFICULTY:"))
        {
            string num = raw.Substring("DIFFICULTY:".Length);
            difficultyText.text = $"DIFFICULTY: {num}";
            return;
        }

        if (raw.StartsWith("RESULT:"))
        {
            var parts = raw.Substring("RESULT:".Length).Split(',');
            string type = parts[0];
            int tWin = int.Parse(parts[1]);
            int tLose = int.Parse(parts[2]);

            if (tWin >= 0) p1Time.text = $"{tWin} ms";
            if (tLose >= 0) p2Time.text = $"{tLose} ms";

            switch (type)
            {
                case "PLAYER1_WIN":
                    sessionP1Score++;
                    p1Panel.color = Color.green;
                    p2Panel.color = Color.red;
                    break;
                case "PLAYER2_WIN":
                    sessionP2Score++;
                    p2Panel.color = Color.green;
                    p1Panel.color = Color.red;
                    break;
                case "DRAW":
                    p1Panel.color = p2Panel.color = Color.yellow;
                    break;
                case "NO_REACTION":
                    p1Panel.color = p2Panel.color = Color.red;
                    break;
            }

            UpdateSessionScoresUI();
        }
    }

    void ResetRoundUI()
    {
        p1Time.text = "--";
        p2Time.text = "--";
        p1Panel.color = p2Panel.color = Color.white;
    }

    void UpdateSessionScoresUI()
    {
        p1Score.text = sessionP1Score.ToString();
        p2Score.text = sessionP2Score.ToString();
    }

    public void RestartScene() => SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
}

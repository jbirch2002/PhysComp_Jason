using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    [Header("UI References")]
    public TMP_Text p1Time, p2Time, p1WinsText, p2WinsText, winnerText;
    public GameObject winScreen;
    public UnityEngine.UI.Slider difficultyMeter;

    int p1Wins => PlayerPrefs.GetInt("P1Wins", 0);
    int p2Wins => PlayerPrefs.GetInt("P2Wins", 0);

    void OnEnable()
    {
        UnitySerialPort.SerialDataParseEvent += OnSerialData;
    }
    void OnDisable()
    {
        UnitySerialPort.SerialDataParseEvent -= OnSerialData;
    }

    void Start()
    {
        UpdateWinsUI();
        winScreen.SetActive(false);
    }

    void OnSerialData(string[] data, string raw)
    {
        // 1) Difficulty: catch the pot line (e.g. "Pot = 2000 -> step delay = 5 ms")
        if (raw.StartsWith("Pot"))
        {
            var idx = raw.IndexOf("step delay = ");
            if (idx >= 0)
            {
                var numPart = raw.Substring(idx + 13).Replace(" ms", "").Trim();
                if (float.TryParse(numPart, out float delay))
                {
                    difficultyMeter.value = 1f - Mathf.InverseLerp(1, 10, delay);
                }
            }
            return;
        }

        if (raw.StartsWith("WINNER:"))
        {
            // raw = "WINNER:1,123"
            var parts = raw.Substring(7).Split(',');
            int player = int.Parse(parts[0]);
            string ms = parts[1];
            ShowResult(player, ms);
        }
        else if (raw.StartsWith("TIE:"))
        {
            var parts = raw.Substring(4).Split(',');
            ShowTie(parts[0], parts[1]);
        }
        else if (raw == "NOREACTION")
        {
            ShowNoReaction();
        }
    }

    void ShowResult(int player, string ms)
    {
        // update that player’s panel
        if (player == 1)
        {
            p1Time.text = ms + " ms";
            PlayerPrefs.SetInt("P1Wins", p1Wins + 1);
        }
        else
        {
            p2Time.text = ms + " ms";
            PlayerPrefs.SetInt("P2Wins", p2Wins + 1);
        }
        PlayerPrefs.Save();
        UpdateWinsUI();
        winnerText.text = $"Player {player} wins in {ms} ms!";
        winScreen.SetActive(true);
    }

    void ShowTie(string ms1, string ms2)
    {
        p1Time.text = ms1 + " ms";
        p2Time.text = ms2 + " ms";
        winnerText.text = $"Tie: {ms1} ms vs {ms2} ms";
        winScreen.SetActive(true);
    }

    void ShowNoReaction()
    {
        winnerText.text = "No reaction – both lose!";
        winScreen.SetActive(true);
    }

    void UpdateWinsUI()
    {
        p1WinsText.text = $"Wins: {p1Wins}";
        p2WinsText.text = $"Wins: {p2Wins}";
    }

    public void RestartScene()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex);
    }
}